/*
 * Copyright (c) 2017 Darren Smith
 *
 * wampcc is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "wampcc/data_model.h"
#include "wampcc/wamp_router.h"

#include <iostream> // TODO: delete me

#define THROW_FOR_INVALID_FN( X ) \
  if (nullptr == m_observer. X) throw std::runtime_error("observer." #X " must be valid");

namespace wampcc {

data_model::data_model()
{
}


data_model::data_model(const data_model& rhs)
{
  /* Note that the list of observers is deliberately not copied, since it would
   * not make sense for an end observer (ie wamp_session or dealer) to be
   * connected to sources of a data model; could lead to conflicting updates. */
}


data_model::~data_model()
{
}


model_topic& data_model::get_topic(const std::string& uri)
{
  std::lock_guard<std::mutex> guard(m_model_topics_mutex);

  auto it = m_model_topic_lookup.find(uri);

  if (it != m_model_topic_lookup.end())
  {
    return *(it->second.get());
  }
  else
  {
    std::unique_ptr<model_topic> uptr(new model_topic(uri, this));
    auto ptr = uptr.get();
    m_model_topic_lookup.insert( std::make_pair(uri, std::move(uptr)) );
    m_model_topics.push_back(ptr);
    return *ptr;
  }
}


void data_model::publish(const json_array& patch,
                         const json_array& rich_event)
{
  /* caller must have locked m_model_topics_mutex */

  for (auto & topic : m_model_topics) {
    try {
      topic->publish_update(patch, rich_event);
    }
    catch (...) { /* ignore exceptions */ }
  }
}

//======================================================================

model_topic::model_topic(std::string uri, data_model * model)
  : m_data_model(model),
    m_uri(uri)
{
}

wampcc::wamp_args model_topic::prepare_snapshot()
{
  json_array patch;

  auto model_snapshot = m_data_model->snapshot();

  json_object& operation = json_append<json_object>(patch);
  operation["op"]    = "replace";
  operation["path"]  = "";  /* replace whole document */
  operation["value"] = std::move(model_snapshot);

  wampcc::wamp_args pub_args;
  pub_args.args_list.push_back( std::move(patch) );

  return pub_args;
}


// TODO: once we have common base class for both wamp_session &
// internal_session, then only one method will be required here.

void model_topic::add_publisher(std::weak_ptr<wamp_session> wp)
{
  /* Lock convention: the model_topics_mutex must be taken and held before the
   * value mutex is taken (the value mutex is take during the snapshot). */
  std::lock_guard<std::mutex> guard(m_data_model->m_model_topics_mutex);

  wampcc::wamp_args pub_args = prepare_snapshot();

  if (auto sp = wp.lock())
  {
    sp->publish(m_uri, { {KEY_PATCH, 1}, {KEY_SNAPSHOT, 1} },
                std::move(pub_args) );
    m_sessions.push_back(sp);
  }
}


void model_topic::add_publisher(std::string realm,
                                std::weak_ptr<wamp_router> dealer)
{
  /* Lock convention: the model_topics_mutex must be taken and held before the
   * value mutex is taken (the value mutex is take during the snapshot). */
  std::lock_guard<std::mutex> guard(m_data_model->m_model_topics_mutex);

  wampcc::wamp_args pub_args = prepare_snapshot();

  if (auto sp = dealer.lock())
  {
    sp->publish(realm, m_uri, { {KEY_PATCH, 1}, {KEY_SNAPSHOT, 1} },
                std::move(pub_args) );
    m_dealers.push_back({realm,sp});
  }
}


void model_topic::publish_update(json_array patch,
                                 json_array event)
{
  wampcc::wamp_args pub_args;
  pub_args.args_list.push_back( std::move(patch) );
  pub_args.args_list.push_back( std::move(event) );

  json_object options{ {KEY_PATCH,1} };

  for (auto & item : m_dealers)
  {
    if (auto sp = item.second.lock())
      try {
        std::cout << "calling dealer publish " << std::endl;
        sp->publish(item.first, m_uri, options, pub_args);
      } catch (...) { /* ignore */ }
  }

  for (auto & item : m_sessions)
  {
    if (auto sp = item.lock())
      try {
        std::cout << "calling wamp_session publish " << std::endl;
        sp->publish(m_uri, options, pub_args);
      } catch (...) { /* ignore */ }
  }
}

//======================================================================

const std::string list_model::key_reset("x");
const std::string list_model::key_insert("i");
const std::string list_model::key_remove("e");
const std::string list_model::key_modify("m");

string_model::string_model(std::string s)
  : m_value( std::move(s) )
{
}


string_model::string_model(const string_model& src)
  : data_model(src),
    m_value(src.value())
{
}


std::string string_model::value() const
{
  std::lock_guard<std::mutex> guard(m_value_mutex);
  return m_value;
}


void string_model::assign(std::string s)
{
  /* Lock convention: the model_topics_mutex must be taken and held before the
   * value mutex is taken (the value mutex is take during the snapshot). */

  std::string tmp;
  std::lock_guard<std::mutex> publisher_guard(m_model_topics_mutex);

  {
    std::lock_guard<std::mutex> value_guard(m_value_mutex);
    if (m_value == s)
      return;
    tmp = s;
    m_value = std::move(s);
  }

  if (!m_model_topics.empty())
  {
    /* Create publication, which comprises two parts:
       (1) the json-model patch
       (2) the rich-model event description.
    */
    json_array patch;
    json_object& operation = json_append<json_object>(patch);
    operation.insert({"op","replace"});
    operation.insert({"path","/body/value"});
    operation.insert({"value",std::move(tmp)});

    // string_model model is so simple that an event description is not needed
    json_array rich_event;

    publish(patch, rich_event);
  }
}


json_value string_model::snapshot() const
{
  internal_impl tmp;
  {
    std::lock_guard<std::mutex> guard(m_value_mutex);
    tmp = m_value;
  }

  json_value jmodel = json_value::make_object();
  json_object & head = json_insert<json_object>(jmodel.as_object(), "head");
  json_object & body = json_insert<json_object>(jmodel.as_object(), "body");
  head.emplace("type",   "string_model");
  head.emplace("version", 0);
  body.insert({"value",std::move(tmp)});

  return jmodel;
}

//======================================================================

model_sub_base::model_sub_base(std::shared_ptr<wampcc::wamp_session>& ws,
                               std::string topic_uri)
  : m_uri(topic_uri),
    m_session(ws)
{
}


model_sub_base::~model_sub_base()
{
}

//======================================================================

jmodel_subscription::jmodel_subscription(std::shared_ptr<wampcc::wamp_session>& ws,
                                         std::string topic_uri,
                                         observer obs)
  : base_type(ws, std::move(topic_uri)),
    m_observer(std::move(obs))
{
  THROW_FOR_INVALID_FN( on_change );
}

void jmodel_subscription::on_update(json_object options,
                                    wamp_args args)
{
  std::cout << "got jmodel update, " << args.args_list << std::endl;
  if (options.find(KEY_PATCH) != options.end())
  {
    auto & patchset = args.args_list[0].as_array();
    {
      std::lock_guard<std::mutex> guard(m_value_mutex);
      m_value.patch(patchset);
      std::cout << "model is now: " << m_value << std::endl;
    }
    //m_observer.on_change();
  }
  else
  {
    // TODO: raise an error?
  }
}

//======================================================================

string_subscription::string_subscription(std::shared_ptr<wampcc::wamp_session>& ws,
                                         std::string topic_uri,
                                         observer ob)
  : base_type(ws, std::move(topic_uri)),
    m_observer(std::move(ob))
{
  THROW_FOR_INVALID_FN( on_change );
}


void string_subscription::on_update(json_object options,
                                    wamp_args args)
{
  /* EV thread */

  /* Note, the string_model model is so simple that an event description is not
   * needed */
  const json_array * patchset = nullptr;
  if (args.args_list.size() > 0 && args.args_list[0].is_array())
    patchset = &args.args_list[0].as_array();

  // TODO: this will be a common pattern to check for a snapshot
  if ( patchset &&
       (patchset->size()==1) &&
       (options.find(KEY_SNAPSHOT) != options.end()) && // is snapshot
       patchset->operator[](0).is_object()
    )
  { //   [ patch ]
    const auto & patch       = patchset->operator[](0).as_object();
    const auto & patch_value = json_get_ref(patch, "value").as_object();
    const auto & body        = json_get_ref(patch_value, "body").as_object();
    const auto & body_value  = json_get_ref(body, "value").as_string();

    {
      std::lock_guard<std::mutex> guard(m_value_mutex);
      m_value = body_value;
    }
    m_observer.on_change(*this);
  }
  else if (patchset)
  {
    const auto & patch = patchset->operator[](0).as_object();
    auto value = std::move(json_get_ref(patch, "value").as_string());

    {
      std::lock_guard<std::mutex> guard(m_value_mutex);
      m_value = std::move(value);
    }

    m_observer.on_change(*this);
  }
}

//======================================================================

list_model::internal_impl list_model::value() const
{
  std::lock_guard<std::mutex> guard(m_value_mutex);
  return m_value;
}


void list_model::reset(internal_impl value)
{
  /* Lock convention: the model_topics_mutex must be taken and held before the
   * value mutex is taken (the value mutex is take during the snapshot). */

  std::lock_guard<std::mutex> pub_guard(m_model_topics_mutex);

  internal_impl copy;
  if (!m_model_topics.empty())
    copy = value;

  {
    std::lock_guard<std::mutex> value_guard(m_value_mutex);
    m_value = std::move(value);
  }

  if (!m_model_topics.empty())
  {
    /* Create publication, which comprises two parts:
       (1) the json-model patch
       (2) the rich-model event description.
    */

    json_array patch;
    json_object& operation = json_append<json_object>(patch);
    operation.insert({"op", "replace"});
    operation.insert({"path", "/body/value"});
    operation.insert({"value", std::move(copy)});;

    // TODO: replace this with the proper event key
    char c = '?';
    json_array rich_event = { c };

    publish(patch, rich_event);
  }
}


void list_model::insert(size_t pos, json_value value)
{
  std::lock_guard<std::mutex> pub_guard(m_model_topics_mutex);
  std::lock_guard<std::mutex> value_guard(m_value_mutex);

  insert_impl(pos, std::move(value));
}


void list_model::push_back(json_value val)
{
  std::lock_guard<std::mutex> pub_guard(m_model_topics_mutex);
  std::lock_guard<std::mutex> value_guard(m_value_mutex);

  insert_impl(m_value.size(), std::move(val));
}


void list_model::insert_impl(size_t pos, json_value value)
{
  if (m_value.size() >= pos)
    m_value.insert(m_value.begin() + pos, value);
  else
    throw bad_index(pos);

  if (!m_model_topics.empty())
  {
    /* Create publication, which comprises two parts:
       (1) the json-model patch
       (2) the rich-model event description.
    */
    json_array patch;
    json_object& operation = json_append<json_object>(patch);
    operation["op"]    = "add";
    operation["path"]  = "/body/value/" + std::to_string(pos);
    operation["value"] = value;

    json_array rich_event = {key_insert, pos};

    publish(patch, rich_event);
  }
}


void list_model::replace(size_t pos, json_value new_value)
{
  std::lock_guard<std::mutex> pub_guard(m_model_topics_mutex);

  json_value copy;
  if (!m_model_topics.empty())
    copy = new_value;

  {
    std::lock_guard<std::mutex> value_guard(m_value_mutex);
    if (m_value.size() > pos)
      m_value[pos] = std::move(new_value);
    else
      throw bad_index(pos);
  }

  if (!m_model_topics.empty())
  {
    /* Create publication, which comprises two parts:
       (1) the json-model patch
       (2) the rich-model event description.
    */

    json_array patch;
    json_object& operation = json_append<json_object>(patch);
    operation["op"]    = "replace";
    operation["path"]  = "/body/value/" + std::to_string(pos);
    operation["value"] = std::move(copy);

    json_array rich_event = {key_modify, pos};

    publish(patch, rich_event);
  }
}


void list_model::erase(size_t pos)
{
  std::lock_guard<std::mutex> pub_guard(m_model_topics_mutex);

  {
    std::lock_guard<std::mutex> value_guard(m_value_mutex);
    if (m_value.size() > pos)
      m_value.erase(m_value.begin() + pos);
    else
      throw bad_index(pos);
  }

  if (!m_model_topics.empty())
  {
    /* Create publication, which comprises two parts:
       (1) the json-model patch
       (2) the rich-model event description.
    */

    json_array patch;
    json_object& operation = json_append<json_object>(patch);
    operation["op"]   = "remove";
    operation["path"] = "/body/value/" + std::to_string(pos);

    json_array rich_event = { key_remove, pos };

    publish(patch, rich_event);
  }
}


json_value list_model::snapshot() const
{
  internal_impl tmp;
  {
    std::lock_guard<std::mutex> guard(m_value_mutex);
    tmp = m_value;
  }

  json_value jmodel = json_value::make_object();
  json_object & head = json_insert<json_object>(jmodel.as_object(), "head");
  json_object & body = json_insert<json_object>(jmodel.as_object(), "body");
  head.insert({"type",   "list_model"}); // TODO: should be a constant
  head.insert({"version",  0 });
  body.insert({"value",std::move(tmp)});

  return jmodel;
}

//======================================================================

list_subscription::list_subscription(std::shared_ptr<wampcc::wamp_session>& ws,
                                     std::string topic_uri,
                                     observer ob)
  : base_type(ws, std::move(topic_uri)),
    m_observer(std::move(ob))
{
  THROW_FOR_INVALID_FN( on_reset );
  THROW_FOR_INVALID_FN( on_insert );
  THROW_FOR_INVALID_FN( on_erase );
  THROW_FOR_INVALID_FN( on_replace );
}


void list_subscription::on_update(json_object details,
                                  wamp_args args)
{
  const json_array& args_list = args.args_list;
  json_value jv = args.args_list;
  //  std::cout << "handler::on_event, " << jv << "\n" << "details:"<< json_value( details ) << "\n";
  const json_array * patch = nullptr;
  const json_array * event = nullptr;

  if (args_list.size() > 0 && args_list[0].is_array())
    patch = &args_list[0].as_array();

  if (args_list.size()>1 && args_list[1].is_array())
    event = &args_list[1].as_array();
  // TODO: check details; needs to have { ... "_p": 1 ... }

  if ( patch &&
       (patch->size()==1) &&
       ( details.find(KEY_SNAPSHOT) != details.end() ) && // is snapshot
       patch->operator[](0).is_object()
    )
  {
    const json_object & patch_replace = patch->operator[](0).as_object();
    const json_object & patch_value   = json_get_ref(patch_replace, "value").as_object();
    const json_object & body          = json_get_ref(patch_value, "body").as_object();
    const json_array  & body_value    = json_get_ref(body, "value").as_array();
    {
      std::lock_guard<std::mutex> guard(m_value_mutex);
      m_value = body_value;
    }
    m_observer.on_reset( *this );
  }
  else if (patch)
  {
    if (event &&
        event->size()>1 &&
        event->at(0).is_string() &&
        event->at(1).is_uint() &&
        event->at(0).as_string() == list_model::key_insert &&
        patch &&
        patch->size()>0 &&
        patch->at(0).is_object()
      )
    {
      auto it = patch->at(0).as_object().find("value");
      if (it != patch->at(0).as_object().end())
      {
        // TODO: handle uint > size_t
        m_observer.on_insert(*this, event->at(1).as_uint());
      }
    }
    else if (event &&
             event->size()>=2 &&
             event->at(0).is_string() &&
             event->at(1).is_uint() &&
             event->at(0).as_string() == list_model::key_remove
      )
    {
      m_observer.on_erase(*this, event->at(1).as_uint());
    }
    else if (event &&
             event->size()>=2 &&
             event->at(0).is_string() &&
             event->at(1).is_int() &&
             event->at(0).as_string() == list_model::key_modify &&
             patch &&
             patch->size()>0 &&
             patch->at(0).is_object()
      )
    {
      auto it = patch->at(0).as_object().find("value");
      if (it != patch->at(0).as_object().end())
      {
        m_observer.on_replace(*this, event->at(1).as_uint());
      }
    }
  }

}

//======================================================================


} // namespace

/*
 * Copyright (c) 2017 Darren Smith
 *
 * wampcc is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef WAMPCC_UTILS_H
#define WAMPCC_UTILS_H

#include <sstream>
#include <list>
#include <random>
#include <memory>
#include <mutex>

#define STRINGIZE2(s) #s
#define STRINGIFY(s) STRINGIZE2(s)

namespace wampcc
{

// replace with optional<> if C++17 present
template <typename T> struct user_optional
{
  user_optional& operator=(T v)
  {
    m_value.first = std::move(v);
    m_value.second = true;
    return *this;
  }
  constexpr T& value() const { return m_value.first; }
  T& value() { return m_value.first; }
  constexpr operator bool() const { return m_value.second; }
private:
  std::pair<T,bool> m_value;
};

struct logger;

enum class HMACSHA256_Mode { HEX, BASE64 };

int compute_HMACSHA256(const char* key, int keylen, const char* msg, int msglen,
                       char* dest, unsigned int* destlen,
                       HMACSHA256_Mode output_mode);


/* must be called with an active exception */
void log_exception(logger& __logptr, const char* callsite);


/* Generate iso8601 timestamp, like YYYY-MM-DDThh:mm:ss.sssZ */
std::string iso8601_utc_timestamp();

/* Generate local timestamp, like "20170527-00:29:48.796000" */
std::string local_timestamp();

/* Generate a random string of ascii printables of length 'len' */
std::string random_ascii_string(const size_t len,
                                unsigned int seed = std::random_device()());

/* Implements a general list of observers, which can be notified with a generic
 * function with variadic arguments. Observer objects should be plain structs
 * that consist of a set of std::function members. */
template <typename T> class observer_list
{
public:
  struct key
  {
  };

  /* Add observer, returning a unique key used for later removal. */
  key* add(T&& obs)
  {
    m_observers.emplace_back(std::unique_ptr<key>(new key()), std::move(obs));
    return m_observers.back().first.get();
  }

  /* Notify observers, by applying a functional object, with supplied
   * arguments. */
  template <typename F, typename... Args>
  void notify(const F& fn, Args&&... args)
  {
    for (auto& item : m_observers)
      fn(item.second, args...);
  }

  void remove(key* const k)
  {
    for (auto it = m_observers.begin(); it != m_observers.end(); ++it) {
      if (k == it->first.get()) {
        m_observers.erase(it);
        return;
      }
    }
  }

  size_t size() const { return m_observers.size(); }
  bool empty() const { return m_observers.empty(); }

private:
  std::vector<std::pair<std::unique_ptr<key>, T>> m_observers;
};


class global_scope_id_generator
{
public:
  static const uint64_t m_min = 0;
  static const uint64_t m_max = 9007199254740992ull;

  global_scope_id_generator() : m_next(0) {}

  uint64_t next()
  {
    if (m_next > m_max)
      m_next = 0;

    return m_next++;
  }

private:
  uint64_t m_next;
};


std::string to_hex(const char* p, size_t size);

/*
 * Tokenize a string based on a single delimiter.
 *
 * want_empty_tokens==true  : include empty tokens, like strsep()
 * want_empty_tokens==false : exclude empty tokens, like strtok()
 */
std::list<std::string> tokenize(const char* src, char delim,
                                bool include_empty_tokens);


inline std::string trim(const std::string& s,
                        const std::string& d = " \f\n\r\t\v")
{
  size_t f = s.find_first_not_of(d);
  if (f == std::string::npos)
    return std::string(); // string is all delims
  else
    return s.substr(f, 1 + s.find_last_not_of(d) - f);
}

inline char* skip_whitespace(char* str)
{
  while (::isspace(*str))
    ++str;
  return str;
}

/* Return whether token exists in src, where token must be bounded on each side
 * by either a string boundary or delim character. */
bool has_token(const std::string& src, const std::string token,
               char delim = ',');

/** Return local hostname, or throw upon failure. */
std::string hostname();

/** Return if the string is a wamp strict URI */
bool is_strict_uri(const char*) noexcept;

class scope_guard
{
public:
  template <class Callable>
  scope_guard(Callable&& undo_func)
    : m_fn(std::forward<Callable>(undo_func))
  {
  }

  scope_guard(scope_guard&& other) : m_fn(std::move(other.m_fn))
  {
    other.m_fn = nullptr;
  }

  ~scope_guard()
  {
    if (m_fn)
      m_fn(); // must not throw
  }

  void dismiss() noexcept { m_fn = nullptr; }

  scope_guard(const scope_guard&) = delete;
  void operator=(const scope_guard&) = delete;

private:
  std::function<void()> m_fn;
};


/** Optionally store a value of value of type T.  Methods to assign
    the value and compare with it are protected by an internal mutex.
*/
template <typename T> class synchronized_optional
{
public:
  synchronized_optional() : m_valid(false) {}

  void set_value(const T& new_value)
  {
    std::lock_guard<std::mutex> guard(m_mutex);
    m_value = new_value;
    m_valid = true;
  }

  void set_value(T&& new_value)
  {
    std::lock_guard<std::mutex> guard(m_mutex);
    m_value = std::move(new_value);
    m_valid = true;
  }

  void release()
  {
    std::lock_guard<std::mutex> guard(m_mutex);
    m_valid = false;
  }

  bool compare(const T& value) const
  {
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_valid && m_value == value;
  }

private:
  mutable std::mutex m_mutex;
  bool m_valid;
  T m_value;
};

// Two byte conversion union
union uint16_converter
{
  uint16_t i;
  uint8_t m[2];
};

// Four byte conversion union
union uint32_converter
{
  uint32_t i;
  uint8_t m[4];
};

// Eight byte conversion union
union uint64_converter
{
  uint64_t i;
  uint8_t m[8];
};


} // namespace wampcc

#endif

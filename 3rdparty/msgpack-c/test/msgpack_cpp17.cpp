#include <msgpack.hpp>

#include <gtest/gtest.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if !defined(MSGPACK_USE_CPP03) && __cplusplus >= 201703

// C++17

TEST(MSGPACK_CPP17, optional_pack_convert_nil)
{
    std::stringstream ss;
    std::optional<int> val1;
    msgpack::pack(ss, val1);
    msgpack::object_handle oh =
        msgpack::unpack(ss.str().data(), ss.str().size());
    std::optional<int> val2 = oh.get().as<std::optional<int> >();
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_CPP17, optional_pack_convert_int)
{
    std::stringstream ss;
    std::optional<int> val1 = 1;
    msgpack::pack(ss, val1);
    msgpack::object_handle oh =
        msgpack::unpack(ss.str().data(), ss.str().size());
    std::optional<int> val2 = oh.get().as<std::optional<int> >();
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_CPP17, optional_pack_convert_vector)
{
    typedef std::optional<std::vector<int> > ovi_t;
    std::stringstream ss;
    ovi_t val1;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    val1 = v;
    msgpack::pack(ss, val1);
    msgpack::object_handle oh =
        msgpack::unpack(ss.str().data(), ss.str().size());
    ovi_t  val2 = oh.get().as<ovi_t>();
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_CPP17, optional_pack_convert_vector_optional)
{
    typedef std::vector<std::optional<int> > voi_t;
    std::stringstream ss;
    voi_t val1;
    val1.resize(3);
    val1[0] = 1;
    val1[2] = 3;
    msgpack::pack(ss, val1);
    msgpack::object_handle oh =
        msgpack::unpack(ss.str().data(), ss.str().size());
    voi_t  val2 = oh.get().as<voi_t>();
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_CPP17, optional_object_nil)
{
    std::optional<int> val1;
    msgpack::object obj(val1);
    std::optional<int> val2 = obj.as<std::optional<int> >();
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_CPP17, optional_object_int)
{
    std::optional<int> val1 = 1;
    msgpack::object obj(val1);
    std::optional<int> val2 = obj.as<std::optional<int> >();
    EXPECT_TRUE(val1 == val2);
}

// Compile error as expected
/*
  TEST(MSGPACK_CPP17, optional_object_vector)
  {
  typedef std::optional<std::vector<int> > ovi_t;
  ovi_t val1;
  std::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  val1 = v;
  msgpack::object obj(val1);
  ovi_t  val2 = obj.as<ovi_t>();
  EXPECT_TRUE(val1 == val2);
  }
*/

TEST(MSGPACK_CPP17, optional_object_with_zone_nil)
{
    msgpack::zone z;
    std::optional<int> val1;
    msgpack::object obj(val1, z);
    std::optional<int> val2 = obj.as<std::optional<int> >();
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_CPP17, optional_object_with_zone_int)
{
    msgpack::zone z;
    std::optional<int> val1 = 1;
    msgpack::object obj(val1, z);
    std::optional<int> val2 = obj.as<std::optional<int> >();
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_CPP17, optional_object_with_zone_vector_optional)
{
    typedef std::vector<std::optional<int> > voi_t;
    msgpack::zone z;
    voi_t val1;
    val1.resize(3);
    val1[0] = 1;
    val1[2] = 3;
    msgpack::object obj(val1, z);
    voi_t  val2 = obj.as<voi_t>();
    EXPECT_TRUE(val1 == val2);
}

struct no_def_con {
    no_def_con() = delete;
    no_def_con(int i):i(i) {}
    int i;
    MSGPACK_DEFINE(i);
};

inline bool operator==(no_def_con const& lhs, no_def_con const& rhs) {
    return lhs.i == rhs.i;
}

inline bool operator!=(no_def_con const& lhs, no_def_con const& rhs) {
    return !(lhs == rhs);
}

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {

template <>
struct as<no_def_con> {
    no_def_con operator()(msgpack::object const& o) const {
        if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
        if (o.via.array.size != 1) throw msgpack::type_error();
        return no_def_con(o.via.array.ptr[0].as<int>());
    }
};

} // adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // msgpack

TEST(MSGPACK_CPP17, optional_pack_convert_no_def_con)
{
    std::stringstream ss;
    std::optional<no_def_con> val1 = no_def_con(1);
    msgpack::pack(ss, val1);
    msgpack::object_handle oh =
        msgpack::unpack(ss.str().data(), ss.str().size());
    std::optional<no_def_con> val2 = oh.get().as<std::optional<no_def_con>>();
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_CPP17, string_view_pack_convert_string_view)
{
    std::stringstream ss;
    std::string s = "ABC";
    std::string_view val1(s);

    msgpack::pack(ss, val1);

    msgpack::object_handle oh;
    msgpack::unpack(oh, ss.str().data(), ss.str().size());
    std::string_view val2 = oh.get().as<std::string_view>();
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_CPP17, string_view_object_strinf_view)
{
    std::string s = "ABC";
    std::string_view val1(s);
    msgpack::object obj(val1);
    std::string_view val2 = obj.as<std::string_view>();
    EXPECT_TRUE(val1 == val2);
}

TEST(MSGPACK_CPP17, string_view_object_with_zone_string_view)
{
    msgpack::zone z;
    std::string s = "ABC";
    std::string_view val1(s);
    msgpack::object obj(val1, z);
    std::string_view val2 = obj.as<std::string_view>();
    EXPECT_TRUE(val1 == val2);
}

#endif // !defined(MSGPACK_USE_CPP03) && __cplusplus >= 201703

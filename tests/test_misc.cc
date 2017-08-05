/*
 * Copyright (c) 2017 Darren Smith
 *
 * wampcc is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "test_common.h"

#include "wampcc/utils.h"
#include "wampcc/platform.h"

#include "mini_test.h"

using namespace wampcc;
using namespace std;

TEST_CASE("test_iso8601_utc_timestamp")
{
  string value = iso8601_utc_timestamp();

  assert(value.size() == 24);
  assert(isdigit(value[0]));
  assert(isdigit(value[1]));
  assert(isdigit(value[2]));
  assert(isdigit(value[3]));
  assert(value[4]=='-');
  assert(isdigit(value[5]));
  assert(isdigit(value[6]));
  assert(value[7]=='-');
  assert(isdigit(value[8]));
  assert(isdigit(value[9]));
  assert(value[10]=='T');
  assert(isdigit(value[11]));
  assert(isdigit(value[12]));
  assert(value[13]==':');
  assert(isdigit(value[14]));
  assert(isdigit(value[15]));
  assert(value[16]==':');
  assert(isdigit(value[17]));
  assert(isdigit(value[18]));
  assert(value[19]=='.');
  assert(isdigit(value[20]));
  assert(isdigit(value[21]));
  assert(isdigit(value[22]));
  assert(value[23]=='Z');
}


void eval_is_strict_uri(const char* src, bool expect)
{
  bool actual = is_strict_uri(src);
  if (actual != expect) {
    cout << "failed:[" << src <<"]"<< std::endl;
    assert(false);
  }
}


TEST_CASE("test_is_strict_uri")
{
  eval_is_strict_uri("a",true);
  eval_is_strict_uri("z",true);
  eval_is_strict_uri("A",true);
  eval_is_strict_uri("Z",true);
  eval_is_strict_uri("0",true);
  eval_is_strict_uri("9",true);
  eval_is_strict_uri("_",true);
  eval_is_strict_uri("azAZ09_",true);
  eval_is_strict_uri("azAZ09_azAZ09_",true);
  eval_is_strict_uri("a.a",true);
  eval_is_strict_uri("z.z",true);
  eval_is_strict_uri("A.A",true);
  eval_is_strict_uri("Z.Z",true);
  eval_is_strict_uri("0.0",true);
  eval_is_strict_uri("9.9",true);
  eval_is_strict_uri("_._",true);
  eval_is_strict_uri("a.a.azAZ09_",true);
  eval_is_strict_uri("z.z.azAZ09_",true);
  eval_is_strict_uri("A.A.azAZ09_",true);
  eval_is_strict_uri("Z.Z.azAZ09_",true);
  eval_is_strict_uri("0.0.azAZ09_",true);
  eval_is_strict_uri("9.9.azAZ09_",true);
  eval_is_strict_uri("_._.azAZ09_",true);
  eval_is_strict_uri("aaa.bbb.ccc",true);

  eval_is_strict_uri("",false);
  eval_is_strict_uri("   ",false);
  eval_is_strict_uri(" ",false);
  eval_is_strict_uri(".",false);
  eval_is_strict_uri("..",false);
  eval_is_strict_uri("...",false);
  eval_is_strict_uri(". .",false);
  eval_is_strict_uri(" . ",false);
  eval_is_strict_uri(" a",false);
  eval_is_strict_uri("a ",false);
  eval_is_strict_uri("a a",false);
  eval_is_strict_uri("a.",false);
  eval_is_strict_uri("z.",false);
  eval_is_strict_uri("A.",false);
  eval_is_strict_uri("Z.",false);
  eval_is_strict_uri("0.",false);
  eval_is_strict_uri("9.",false);
  eval_is_strict_uri("_.",false);
  eval_is_strict_uri(".a",false);
  eval_is_strict_uri(".z",false);
  eval_is_strict_uri(".A",false);
  eval_is_strict_uri(".Z",false);
  eval_is_strict_uri(".0",false);
  eval_is_strict_uri(".9",false);
  eval_is_strict_uri("._",false);
  eval_is_strict_uri("aaa.bbb.ccc.",false);
  eval_is_strict_uri("aaa.bbb..ccc",false);
  eval_is_strict_uri("aaa..bbb.ccc",false);
  eval_is_strict_uri(".aaa.bbb.ccc",false);
  eval_is_strict_uri("aaa...bbb...ccc",false);
  eval_is_strict_uri("aaa. bbb.ccc",false);
  eval_is_strict_uri("aaa.b bb.ccc",false);
  eval_is_strict_uri("aaa.bbb .ccc",false);
  eval_is_strict_uri("aaa.bbb.ccc ",false);
  eval_is_strict_uri(" aaa.bbb.ccc",false);
  eval_is_strict_uri("a.a..azAZ09_",false);
  eval_is_strict_uri("z.z..azAZ09_",false);
  eval_is_strict_uri("A.A..azAZ09_",false);
  eval_is_strict_uri("Z.Z..azAZ09_",false);
  eval_is_strict_uri("0.0..azAZ09_",false);
  eval_is_strict_uri("9.9..azAZ09_",false);
  eval_is_strict_uri("_._..azAZ09_",false);
  eval_is_strict_uri("a.a.a zAZ09_",false);
  eval_is_strict_uri(" z.z.azAZ09_",false);
  eval_is_strict_uri("A. A.azAZ09_",false);
  eval_is_strict_uri("Z.Z. azAZ09_",false);
  eval_is_strict_uri("0.0.az AZ09_",false);
  eval_is_strict_uri("9.9.azAZ 09_",false);
  eval_is_strict_uri("_._.azAZ09 _",false);
  eval_is_strict_uri("a.a.az#AZ09_",false);
  eval_is_strict_uri("z.z.az!AZ09_",false);
  eval_is_strict_uri("A.A.az£AZ09_",false);
  eval_is_strict_uri("Z.Z.az$AZ09_",false);
  eval_is_strict_uri("0.0.az%AZ09_",false);
  eval_is_strict_uri("9.9.az^AZ09_",false);
  eval_is_strict_uri("_._.az&AZ09_",false);
  eval_is_strict_uri("_._.az[AZ09_",false);
  eval_is_strict_uri("_._.az]AZ09_",false);
  eval_is_strict_uri("_._.az(AZ09_",false);
  eval_is_strict_uri("_._.az)AZ09_",false);
  eval_is_strict_uri("_._.az:AZ09_",false);
  eval_is_strict_uri("_._.az\\AZ09_",false);
  eval_is_strict_uri("_._.az/AZ09_",false);
  eval_is_strict_uri("_._.az<AZ09_",false);
  eval_is_strict_uri("_._.az>AZ09_",false);
  eval_is_strict_uri("_._.az,AZ09_",false);
  eval_is_strict_uri("_._.azA¬Z09_",false);
  eval_is_strict_uri("_._.azA`Z09_",false);
  eval_is_strict_uri("_._.az~AZ09_",false);
}


TEST_CASE("test_localtime")
{
	auto str = wampcc::local_timestamp();
	assert(str.size() == 24);
	assert(str[8] == '-');
	assert(str[11] == ':');
	assert(str[14] == ':');
	assert(str[17] == '.');
}


TEST_CASE("test_time_now")
{
	auto t1 = time_now();
	auto t2 = time_now();

	assert((t2.sec - t1.sec) < 2);
	assert(t1.sec > 1495835714LL);
	assert(t1.usec >= 0);
	assert(t1.usec < 1000000);
}


int main(int argc, char** argv)
{
  try {
    int result = minitest::run(argc, argv);
    return (result < 0xFF ? result : 0xFF );
  } catch (exception& e) {
    cout << e.what() << endl;
    return 1;
  }
}




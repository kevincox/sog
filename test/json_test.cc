#include "gtest/gtest.h"
#include "sog/init.h"
#include "sog/json.h"
#include "sog/macro.h"

struct WithJsonLogger {
	std::stringstream buf;
	sog::JsonSink sink;
	
	WithJsonLogger():
		buf{},
		sink{&buf}
	{
		sog::init(&sink);
	}
	
	std::string next() {
		std::stringbuf line;
		buf.get(line);
		EXPECT_EQ(buf.get(), '\n');
		return line.str();
	}
} logger;

struct Json: public ::testing::Test  {
	void SetUp() override {
		std::stringstream tmp;
		logger.buf.swap(tmp);
	}
	void TearDown() override {
		EXPECT_EQ(logger.buf.get(), std::stringstream::traits_type::eof());
		EXPECT_TRUE(logger.buf.eof());
	}
};

TEST_F(Json, Basic) {
	LOG(INFO, "$msg and $other", msg, "Foo", other, "bar");
	EXPECT_EQ(logger.next(), "{"
		R""("_msg":"Foo and bar",)""
		R""("_file":"test/json_test.cc",)""
		R""("_line":1337,)""
		R""("_func":"virtual void Json_Basic_Test::TestBody()",)""
		R""("_level":3,)""
		R""("_tmpl":"$msg and $other",)""
		R""("_sec":950455800,)""
		R""("_usec":50000,)""
		R""("msg":"Foo",)""
		R""("other":"bar"})"");
	
	LOG(INFO, "", msg, 5, other, "fart");
	EXPECT_EQ(logger.next(), "{"
		R""("_msg":"",)""
		R""("_file":"test/json_test.cc",)""
		R""("_line":1337,)""
		R""("_func":"virtual void Json_Basic_Test::TestBody()",)""
		R""("_level":3,)""
		R""("_tmpl":"",)""
		R""("_sec":950455800,)""
		R""("_usec":50000,)""
		R""("msg":5,)""
		R""("other":"fart"})"");
}

#include <sstream>

#include "sog/init.h"
#include "sog/pretty.h"
#include "sog/macro.h"
#include "gtest/gtest.h"

struct WithPrettyLogger {
	std::stringstream buf;
	sog::PrettySink sink;
	
	WithPrettyLogger():
		buf{},
		sink{&buf}
	{
		sog::init(&sink);
	}
	
	std::string next_line() {
		std::stringbuf line;
		buf.get(line);
		EXPECT_EQ(buf.get(), '\n');
		return line.str();
	}
} logger;

struct Pretty: public ::testing::Test  {
	void SetUp() override {
		std::stringstream tmp;
		logger.buf.swap(tmp);
	}
	void TearDown() override {
		EXPECT_EQ(logger.buf.get(), std::stringstream::traits_type::eof());
		EXPECT_TRUE(logger.buf.eof());
	}
};

TEST_F(Pretty, Basic) {
	LOG(INFO, "A thing");
	EXPECT_EQ(logger.next_line(), "2000-02-13T15:30:00 I pretty_test.cc:1337 A thing");
	
	LOG(ERROR, "Another thing");
	EXPECT_EQ(logger.next_line(), "2000-02-13T15:30:00 E pretty_test.cc:1337 Another thing");
	
	LOG(WARN, "  COOL  ");
	EXPECT_EQ(logger.next_line(), "2000-02-13T15:30:00 W pretty_test.cc:1337   COOL  ");
}

TEST_F(Pretty, Formatting) {
	LOG(INFO, "Foo: $foo", foo, "a foo", bar, "baz");
	EXPECT_EQ(logger.next_line(), "2000-02-13T15:30:00 I pretty_test.cc:1337 Foo: a foo");
	
	LOG(9, " |$val| ", val, "foo");
	EXPECT_EQ(logger.next_line(), "2000-02-13T15:30:00 T pretty_test.cc:1337  |foo| ");
	
	LOG(WARN, "$a $a $b $a", a, "A", b, "B", c, "C");
	EXPECT_EQ(logger.next_line(), "2000-02-13T15:30:00 W pretty_test.cc:1337 A A B A");
}

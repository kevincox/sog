#include "with_memory_logger.h"

TEST_F(Sog, Format) {
	LOG(INFO, "$msg and $other", msg, "Foo", other, "bar");
	EXPECT_EQ(logger.take_formatted(), "Foo and bar");
	
	LOG(INFO, "$a and $a", a, "Foo", b, "bar");
	EXPECT_EQ(logger.take_formatted(), "Foo and Foo");
	
	LOG(INFO, " $bar ", bar, "Foo");
	EXPECT_EQ(logger.take_formatted(), " Foo ");
}

TEST_F(Sog, FormatTypes) {
	LOG(INFO, "$int $float $str", int, 5, float, 3.1415, str, "a string");
	EXPECT_EQ(logger.take_formatted(), "5 3.1415 a string");
}

TEST_F(Sog, FormatCheck) {
#if defined(__GNUC__) && !defined(__clang__)
	return;
#endif
	
	std::vector<std::string_view> keys {"a", "foo", "a_str-key"};
	sog::Source s { sog::level::INFO, __FILE__, "func", __LINE__, "", 3, keys.data()};
	EXPECT_TRUE((s.msg_template = "A msg", sog::_is_valid(&s)));
	EXPECT_TRUE((s.msg_template = "My $foo is foo", sog::_is_valid(&s)));
	EXPECT_TRUE((s.msg_template = "$a_str-key My $foo is $a", sog::_is_valid(&s)));
	EXPECT_TRUE((s.msg_template = "$$$foo", sog::_is_valid(&s)));
	EXPECT_TRUE((s.msg_template = "$$bar", sog::_is_valid(&s)));
	
	EXPECT_ANY_THROW((s.msg_template = "My $bar is a foo.", sog::_is_valid(&s)));
	EXPECT_ANY_THROW((s.msg_template = "My $foo is a foo$", sog::_is_valid(&s)));
	EXPECT_ANY_THROW((s.msg_template = "My $ is a foo$", sog::_is_valid(&s)));
}

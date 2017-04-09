#include "util.h"

TEST_F(Sog, Format) {
	LOG(INFO, "$msg and $other", msg, "Foo", other, "bar");
	EXPECT_EQ(logger.take_formatted(), "Foo and bar");
	
	LOG(INFO, "$a and $a", a, "Foo", b, "bar");
	EXPECT_EQ(logger.take_formatted(), "Foo and Foo");
}

TEST_F(Sog, FormatCheck) {
	std::vector<std::experimental::string_view> keys {"a", "foo", "a_str-key"};
	sog::Source s { __FILE__, "func", __LINE__, "", 3, keys.data()};
	EXPECT_TRUE((s.msg_template = "A msg", sog::_is_valid(&s)));
	EXPECT_TRUE((s.msg_template = "My $foo is foo", sog::_is_valid(&s)));
	EXPECT_TRUE((s.msg_template = "$a_str-key My $foo is $a", sog::_is_valid(&s)));
	EXPECT_TRUE((s.msg_template = "$$$foo", sog::_is_valid(&s)));

	EXPECT_FALSE((s.msg_template = "My $bar is a foo.", sog::_is_valid(&s)));
	EXPECT_FALSE((s.msg_template = "My $bar is a foo$", sog::_is_valid(&s)));
	EXPECT_FALSE((s.msg_template = "My $ is a foo$", sog::_is_valid(&s)));
}

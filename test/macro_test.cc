#include "with_memory_logger.h"

TEST_F(Sog, Macro) {
	std::string secret = "top secret";
	LOG(INFO, "My $msg is foo",
		msg, "Foo",
		other, secret);
	
	EXPECT_LOG("My $msg is foo", {"msg", "Foo"}, {"other", "top secret"});
}

TEST_F(Sog, MacroMultiScope) {
	std::string secret = "top";
	secret += " secret";
	
	LOG(INFO, "Var: $var",
		var, secret,
		other, secret + " bar");
	
	if (true) {
		LOG(WARN, "If: $other",
			var, secret,
			other, "foo");
	
		LOG(WARN, "If: $var",
			var, 5);
	}
	
	LOG(INFO, "Last: $var",
		var, secret + "ive",
		another, "");
	
	EXPECT_LOG("Var: $var", {"var", "top secret"}, {"other", "top secret bar"});
	EXPECT_LOG("If: $other", {"var", "top secret"}, {"other", "foo"});
	EXPECT_LOG("If: $var", {"var", 5});
	EXPECT_LOG("Last: $var", {"var", "top secretive"}, {"another", ""});
}

TEST_F(Sog, MacroEvalOnce) {
	std::string val = "";
	
	LOG(INFO, "Thing: $val",
		val, val += "foo");
	
	EXPECT_EQ(val, "foo");
	
	LOG(INFO, "Thing: $val",
		val, val += "foo");
	
	EXPECT_EQ(val, "foofoo");
	
	EXPECT_LOG("Thing: $val", {"val", "foo"});
	EXPECT_LOG("Thing: $val", {"val", "foofoo"});
}

TEST_F(Sog, MacroSavesTemp) {
	std::string sso = "sh";
	std::string heap = "string that is longer then SSO strings";
	
	LOG(INFO, "Log",
		sso, sso + "rt",
		heap, heap + " and another bit.");
	
	EXPECT_LOG("Log",
		{"sso", "shrt"},
		{"heap", "string that is longer then SSO strings and another bit."});
}

TEST_F(Sog, MacroLoop) {
	for (int i = 0; i < 3; ++i)
		LOG(INFO, "!", i, std::to_string(i));
	
	EXPECT_LOG("!", {"i", "0"});
	EXPECT_LOG("!", {"i", "1"});
	EXPECT_LOG("!", {"i", "2"});
}

#define COMMON \
	foo, "bar", \
	version, 5, \
	flarb, 1.234

TEST(MemoryLogger, Macro) {
	LOG(WARN, "$foo $version", COMMON);
	EXPECT_LOG("$foo $version",
		{"foo", "bar"},
		{"version", 5},
		{"flarb", 1.234});
	
	LOG(WARN, "$custom $foo $uncommon",
		custom, 492,
		COMMON,
		uncommon, "blah");
	EXPECT_LOG("$custom $foo $uncommon",
		{"custom", 492},
		{"foo", "bar"},
		{"version", 5},
		{"flarb", 1.234},
		{"uncommon", "blah"});
}

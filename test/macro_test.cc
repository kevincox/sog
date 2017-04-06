#include <gtest/gtest.h>
#include "test/util.h"
#include "sog/sog.h"

WithMemoryLogger logger;

TEST(Macro, Basic) {
	std::string secret = "top secret";
	LOG(INFO, "My $msg is foo",
		msg, "Foo",
		other, secret);
	
	EXPECT_EQ(logger.take_pairs(), WithMemoryLogger::Pairs({
		{ {"msg", "Foo"},   {"other", "top secret"} },
	}));
}

TEST(Macro, MultiScope) {
	std::string secret = "top";
	secret += " secret";
	
	LOG(INFO, "Var: $var",
		var, secret,
		other, secret + " bar");
	
	if (true) {
		LOG(WARN, "If: $other",
			var, secret,
			other, "foo");
	}
	
	LOG(INFO, "Last: $var",
		var, secret + "ive",
		another, "");
	
	EXPECT_EQ(logger.take_pairs(), WithMemoryLogger::Pairs({
		{ {"var", "top secret"},   {"other", "top secret bar"} },
		{ {"var", "top secret"},   {"other", "foo"} },
		{ {"var", "top secretive"}, {"another", ""} },
	}));
}

TEST(Macro, EvalOnce) {
	std::string val = "";
	
	LOG(INFO, "Thing: $val",
		val, val += "foo");
	
	EXPECT_EQ(val, "foo");
	
	LOG(INFO, "Thing: $val",
		val, val += "foo");
	
	EXPECT_EQ(val, "foofoo");
	
	EXPECT_EQ(logger.take_pairs(), WithMemoryLogger::Pairs({
		{ {"val", "foo"} },
		{ {"val", "foofoo"} },
	}));
}

TEST(Macro, SavesTemp) {
	std::string sso = "sh";
	std::string heap = "string that is longer then SSO strings";
	
	LOG(INFO, "Log",
		sso, sso + "rt",
		heap, heap + " and another bit.");
	
	EXPECT_EQ(logger.take_pairs(), WithMemoryLogger::Pairs({
		{
			{"sso", "shrt"},
			{"heap", "string that is longer then SSO strings and another bit."}
		},
	}));
}

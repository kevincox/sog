#include <gtest/gtest.h>
#include "test/util.h"
#include "sog/sog.h"

WithMemoryLogger logger;

TEST(MemoryLogger, Basic) {
	std::string secret = "top secret";
	LOG(INFO, "My $msg is foo",
		msg = "Foo",
		other = secret);
	
	EXPECT_EQ(logger.take_pairs(), WithMemoryLogger::Pairs({
		{ {"msg", "Foo"},   {"other", "top secret"} },
	}));
}

TEST(MemoryLogger, MultiScope) {
	std::string secret = "top";
	secret += " secret";
	
	LOG(INFO, "Var: $var",
		var = secret,
		other = secret + " bar");
	
	if (true) {
		LOG(WARN, "If: $other",
			var = secret,
			other = "foo");
	}
	
	LOG(INFO, "Last: $var",
		var = secret + "ive",
		another = "");
	
	EXPECT_EQ(logger.take_pairs(), WithMemoryLogger::Pairs({
		{ {"var", "top secret"},   {"other", "top secret bar"} },
		{ {"var", "top secret"},   {"other", "foo"} },
		{ {"var", "top secretive"}, {"another", ""} },
	}));
}

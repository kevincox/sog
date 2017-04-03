#include <gtest/gtest.h>
#include "test/util.h"
#include "sog/dynamic.h"

WithMemoryLogger logger;

TEST(MemoryLogger, Basic) {
	sog::DynamicLog log1;
	log1.add("foo", "bar");
	log1.add("bar", "two");
	log1.add("baz", "c");
	
	log1.submit();
	
	EXPECT_EQ(logger.take_pairs(), WithMemoryLogger::Pairs({
		{ {"foo", "bar"}, {"bar", "two"}, {"baz", "c"} },
	}));
	
	sog::DynamicLog log2;
	log2.add("a", "1");
	log2.add("b", "2");
	log2.add("c", "3");
	log2.submit();
	sog::DynamicLog log3;
	log3.add("foo", "x");
	log3.add("bar", "y");
	log3.add("baz", "z");
	log3.submit();
	
	EXPECT_EQ(logger.take_pairs(), WithMemoryLogger::Pairs({
		{ {"a", "1"},   {"b", "2"},   {"c", "3"}   },
		{ {"foo", "x"}, {"bar", "y"}, {"baz", "z"} },
	}));
}

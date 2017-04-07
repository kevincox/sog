#include "test/util.h"

#include <gtest/gtest.h>
#include "sog/sog.h"

WithMemoryLogger logger;

void count(int n) {
	if (!n--)
		return;
	
	LOG(INFO, "$n more", n, std::to_string(n));
	return count(n);
}

TEST(Macro, Func) {
	LOG(INFO, "Calling 3 times.");
	count(3);
	
	EXPECT_EQ(logger.take_pairs(), WithMemoryLogger::Pairs({
		{
			{"FILE", __FILE__},
			{"LINE", "17"},
			{"FUNC", "virtual void Macro_Func_Test::TestBody()"},
			{"MSG", "Calling 3 times."},
		}, {
			{"FILE", __FILE__},
			{"LINE", "12"},
			{"FUNC", "void count(int)"},
			{"MSG", "$n more"},
			{"n", "2"}
		}, {
			{"FILE", __FILE__},
			{"LINE", "12"},
			{"FUNC", "void count(int)"},
			{"MSG", "$n more"},
			{"n", "1"}
		}, {
			{"FILE", __FILE__},
			{"LINE", "12"},
			{"FUNC", "void count(int)"},
			{"MSG", "$n more"},
			{"n", "0"}
		},
	}));
}

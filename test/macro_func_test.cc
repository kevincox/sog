#include "test/util.h"

void count(int n) {
	if (!n--)
		return;
	
	LOG(INFO, "$n more", n, std::to_string(n));
	return count(n);
}

TEST_F(Sog, MacroFunc) {
	LOG(INFO, "Calling 3 times.");
	count(3);
	
	EXPECT_EQ(logger.take_pairs(),
		Pairs({
			{"FILE", __FILE__},
			{"LINE", "12"},
			{"FUNC", "virtual void Sog_MacroFunc_Test::TestBody()"},
			{"MSG", "Calling 3 times."}}));
	EXPECT_EQ(logger.take_pairs(),
		Pairs({
			{"FILE", __FILE__},
			{"LINE", "7"},
			{"FUNC", "void count(int)"},
			{"MSG", "$n more"},
			{"n", "2"}}));
	EXPECT_EQ(logger.take_pairs(),
		Pairs({
			{"FILE", __FILE__},
			{"LINE", "7"},
			{"FUNC", "void count(int)"},
			{"MSG", "$n more"},
			{"n", "1"}}));
	EXPECT_EQ(logger.take_pairs(),
		Pairs({
			{"FILE", __FILE__},
			{"LINE", "7"},
			{"FUNC", "void count(int)"},
			{"MSG", "$n more"},
			{"n", "0"}}));
	EXPECT_NO_LOG();
}

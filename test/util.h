#include <vector>

#include <gtest/gtest.h>

#include "sog/macro.h"
#include "sog/init.h"
#include "sog/memory_logger.h"

#define EXPECT_LOG(msg, ...) \
	EXPECT_EQ(logger.take_pairs(), Pairs({ \
	{"FILE", __FILE__}, \
	{"LINE", std::to_string(_sog_LINE)}, \
	{"FUNC", __PRETTY_FUNCTION__}, \
	{"MSG", msg}, \
	__VA_ARGS__ }))

#define EXPECT_NO_LOG() \
	EXPECT_EQ(logger.memory_logger->messages.size(), 0)

using Pairs = std::vector<std::pair<std::string,sog::OwnedValue>>;

struct WithMemoryLogger {
	sog::MemoryLogger *memory_logger;
	WithMemoryLogger();
	
	sog::OwnedMessage take();
	Pairs take_pairs();
	void clear();
	std::string take_formatted();
};

extern WithMemoryLogger logger;

struct Sog: public ::testing::Test  {
	void SetUp() override;
	void TearDown() override;
};

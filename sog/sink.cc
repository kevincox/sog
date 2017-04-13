#include "sink.h"

constexpr std::experimental::string_view LEVEL_NAMES[] = {
	{ "FTL", 3 },
	{ "ERR", 3 },
	{ "WRN", 3 },
	{ "INF", 3 },
	{ "DBG", 3 },
	{ "TRA", 3 },
};
constexpr size_t LEVEL_NAMES_LEN = sizeof(LEVEL_NAMES) / sizeof(*LEVEL_NAMES);

std::experimental::string_view sog::level_name(sog::Level l) {
	size_t i = l;
	if (i >= LEVEL_NAMES_LEN)
		i = LEVEL_NAMES_LEN - 1;
	return LEVEL_NAMES[i];
}

constexpr char LEVEL_CHARS[] = {
	'F',
	'E',
	'W',
	'I',
	'D',
	'T',
};
constexpr size_t LEVEL_CHARS_LEN = sizeof(LEVEL_CHARS) / sizeof(*LEVEL_CHARS);

char sog::level_char(sog::Level l) {
	size_t i = l;
	if (i >= LEVEL_CHARS_LEN)
		i = LEVEL_CHARS_LEN - 1;
	return LEVEL_CHARS[i];
}

std::unique_ptr<sog::SinkData> sog::Sink::prepare(const Source *source) {
	return nullptr;
}

#include "sink.h"

constexpr std::string_view LEVEL_NAMES[] = {
	{ "FTL", 3 },
	{ "ERR", 3 },
	{ "WRN", 3 },
	{ "INF", 3 },
	{ "DBG", 3 },
	{ "TRA", 3 },
};
constexpr size_t LEVEL_NAMES_LEN = sizeof(LEVEL_NAMES) / sizeof(*LEVEL_NAMES);

std::string_view sog::level_name(sog::Level l) {
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

sog::Level sog::level_value(std::string_view s) {
	if (s == "FATAL") return level::FATAL;
	if (s == "fatal") return level::FATAL;
	if (s == "ERROR") return level::ERROR;
	if (s == "error") return level::ERROR;
	if (s == "WARN") return level::WARN;
	if (s == "warn") return level::WARN;
	if (s == "INFO") return level::INFO;
	if (s == "info") return level::INFO;
	if (s == "TRACE") return level::TRACE;
	if (s == "trace") return level::TRACE;
	if (s == "ALL") return level::TRACE;
	if (s == "all") return level::TRACE;
	try {
		size_t end;
		auto i = std::stoul(std::string(s), &end);
		if (end != s.size())
			return LEVEL_ERROR;
		if (i > std::numeric_limits<Level>::max())
			return ENABLE_ALL;
		return i;
	} catch (std::out_of_range oor) {
		return ENABLE_ALL;
	} catch (std::invalid_argument ia) {
		return LEVEL_ERROR;
	}
	return LEVEL_ERROR;
}

sog::Level sog::Sink::default_log_level() {
	return level::INFO;
}

sog::Prepared sog::Sink::prepare(const Source *source) {
	return { nullptr };
}

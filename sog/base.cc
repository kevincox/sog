#include "init.h"
#include "pretty.h"

using string_view = std::experimental::string_view;

namespace {

sog::Value::Data data_ref(const sog::Value &v) {
	auto *str = boost::get<std::string>(&v.data);
	if (str)
		return sog::Value::Data(std::experimental::string_view{*str});
	return v.data;
}

struct LevelOpt {
	std::string suffix;
	sog::Level level;
};

sog::Sink *sink = nullptr;
sog::Level min_log_level;
std::vector<LevelOpt> custom_levels;

}

sog::SinkData::~SinkData() {}

bool sog::Value::operator==(const Value &that) const {
	auto l = data_ref(*this);
	auto r = data_ref(that);
	// return data_ref(*this) == data_ref(that);
	return l == r;
}

void sog::init(sog::Sink *newsink) {
	assert(newsink != nullptr);
	
	if (!newsink)
		newsink = new sog::PrettySink(&std::clog);
	
	std::experimental::string_view env_level{std::getenv("SOG_LEVEL")};
	min_log_level = newsink->default_log_level();
	while (!env_level.empty()) {
		auto comma = env_level.find(',');
		auto opt = env_level.substr(0, comma);
		auto equals = opt.rfind('=');
		if (equals == opt.npos) {
			min_log_level = level_value(opt);
		} else {
			LevelOpt levelopt;
			levelopt.suffix = std::string(opt.substr(0, equals));
			levelopt.level = level_value(opt.substr(equals + 1));
			custom_levels.emplace_back(std::move(levelopt));
		}
		
		if (comma == env_level.npos)
			break;
		
		env_level = env_level.substr(comma + 1);
	}
	
	assert(sink == nullptr);
	sink = newsink;
}

sog::Prepared sog::_prepare(const Source *source) {
	assert(((void)"sog::init() must be called before any log messages.", sink));
	
	if (source->level > min_log_level)
		return {nullptr, false};
	
	return sink->prepare(source);
}

void sog::_submit(SinkData *d, Message m) {
	sink->log(d, m);
	if (m.source->level == level::FATAL)
		std::terminate();
}

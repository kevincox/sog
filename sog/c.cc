extern "C" {
#include "c.h"
}

#include "init.h"

namespace {
	sog::Value to_value(const SogValue &val) {
		switch (val.type) {
			case SOG_LONG: return val.val_long;
			case SOG_DOUBLE: return val.val_double;
			case SOG_STR: return {val.val_str.data, val.val_str.size};
			default:
				fprintf(stderr, "Unknown type %d for SogValue\n", val.type);
				abort();
		}
	}
}

struct SogSource {
	sog::Source source;
	std::vector<std::string_view> keys;
	sog::Prepared prepared;
};

void sog_init_pretty() {
	sog::init();
}

SogSource *sog_source_new(
	uint16_t level,
	SogStr file,
	SogStr function,
	uint32_t line,
	SogStr msg_template,
	size_t value_count,
	SogStr *keys)
{
	auto r = std::make_unique<SogSource>();
	
	r->keys.reserve(value_count);
	for (size_t i = 0; i < value_count; ++i)
		r->keys.emplace_back(keys[i].data, keys[i].size);
	
	r->source.level = level;
	r->source.file = {file.data, file.size};
	r->source.function = {function.data, function.size};
	r->source.line = line;
	r->source.msg_template = {msg_template.data, msg_template.size};
	r->source.value_count = value_count;
	r->source.keys = r->keys.data();
	
	r->prepared = sog::_prepare(&r->source);
	
	return r.release();
}

void sog_source_delete(struct SogSource *source) {
	delete source;
}

void sog_submit(SogSource *source, const SogValue *values) {
	if (!source->prepared.log) return;
	
	std::vector<sog::Value> vals;
	vals.reserve(source->source.value_count);
	for (size_t i = 0; i < source->source.value_count; ++i)
		vals.emplace_back(to_value(values[i]));
	
	sog::Message msg = {&source->source, vals.data()};
	
	sog::_submit(source->prepared.sink_data.get(), std::move(msg));
}

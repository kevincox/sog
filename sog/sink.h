#ifndef SOG___SINK_H
#define SOG___SINK_H

#include "base.h"

namespace sog {
	struct Sink {
		virtual ~Sink() {};
		virtual std::unique_ptr<SinkData> prepare(const Source *source);
		virtual void log(SinkData *sd, Message msg) = 0;
	};
	
	std::experimental::string_view level_name(sog::Level l);
	char level_char(sog::Level l);
	
	using OwnedValue = std::string;
	
	struct OwnedMessage {
		const Source *source;
		std::vector<OwnedValue> values;
		
		explicit OwnedMessage(const Message &msg):
			source(msg.source)
		{
			values.reserve(source->value_count);
			for (size_t i = 0; i < source->value_count; ++i)
				values.emplace_back(msg.values[i]);
		}
	};
}

#endif

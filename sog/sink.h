#ifndef SOG___SINK_H
#define SOG___SINK_H

#include "base.h"

namespace sog {
	using OwnedValue = std::string;
	
	struct OwnedMessage {
		const Source *source;
		std::vector<OwnedValue> values;
		
		OwnedMessage(const Message &msg):
			source(msg.source)
		{
			values.reserve(source->value_count);
			for (size_t i = 0; i < source->value_count; ++i)
				values.emplace_back(msg.values[i]);
		}
	};
}

#endif

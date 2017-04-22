#ifndef SOG_SINK_H
#define SOG_SINK_H

#include "base.h"

namespace sog {
	inline std::ostream &operator<<(std::ostream &out, const sog::Value &v) {
		return out << v.data;
	}
	
	/** Get the name of a log level.
	 * 
	 * Returns a string_view to a static string describing the log level. Note
	 * that some log levels return the same string (in particular high log
	 * levels return TRACE irrespective of their specific value).
	 */
	std::experimental::string_view level_name(sog::Level level);
	
	/** Get a character representing a log level.
	 * 
	 * Note that the returned charcter isn't necessarily unique for a give log
	 * level (see sog::level_name()).
	 */
	char level_char(sog::Level l);
	
	struct OwnedMessage {
		const Source *source;
		std::vector<Value> values;
		
		OwnedMessage(Message msg):
			source(msg.source)
		{
			values.reserve(source->value_count);
			for (size_t i = 0; i < source->value_count; ++i)
				values.emplace_back(msg.values[i].owned());
		}
		
		Message ref() {
			return { source, values.data() };
		}
	};
}

#endif

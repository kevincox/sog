#ifndef SOG_SINK_H
#define SOG_SINK_H

#include "base.h"

namespace sog {
	inline std::ostream &operator<<(std::ostream &out, const sog::Value &v) {
		return out << v.data;
	}
	
	// Some information passed from the sink to the library.
	struct Logged {
	};
	
	/** Sink interface.
	 *
	 * This is the interface that should be written to for consuming messages
	 * logged with sog. It is not expected that users will call this interface
	 * themselves.
	 */
	struct Sink {
		virtual ~Sink() {}
		
		/** Returned the default log level for this sink.
		 * 
		 * This is used in sog::init() to setup a filter of sources that won't
		 * be passed to Sink::prepare().
		 */
		virtual Level default_log_level();
		
		/** Prepare data for a source.
		 *
		 * This method will be called once for every source. The value returned
		 * will be deleted if the source is ever destructed. Note that in normal
		 * use of sog sources are never destructed but there is an API provided
		 * for "dynamic" logging in which the destruction of sources is common.
		 *
		 * @warning This function must be reentrant. It will only be called once
		 *	for any given source but it may be called for multiple sources
		 *	concurrently.
		 *
		 * @param source The source to prepare for.
		 * @return Arbitrary data.
		 */
		virtual Prepared prepare(const Source *source);
		
		/** Log a message.
		 *
		 * This message provides the sink data from a previous call to prepare
		 * and a sog::Message. The msg.source attribute will be the same source
		 * (both in location and content) as the source passed to the prepare()
		 * call that produced sink_data.
		 *
		 * @warning This function must be reentrant. It will be called
		 *	concurrently for the same and different sinks.
		 *
		 * @param sink_data Data from a previous call to prepare()
		 * @param msg The message to log.
		 */
		virtual Logged log(SinkData *sink_data, Message msg) = 0;
	};
	
	/** Get the name of a log level.
	 *
	 * Returns a string_view to a static string describing the log level. Note
	 * that some log levels return the same string (in particular high log
	 * levels return TRACE irrespective of their specific value).
	 */
	std::string_view level_name(sog::Level level);
	
	/** Get a character representing a log level.
	 *
	 * Note that the returned charcter isn't necessarily unique for a give log
	 * level (see sog::level_name()).
	 */
	char level_char(sog::Level l);
	
	/** Parse level name into sog::Level
	 */
	Level level_value(std::string_view s);
	
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

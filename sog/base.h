#ifndef SOG___BASE_H
#define SOG___BASE_H

#include <assert.h>
#include <experimental/string_view>
#include <initializer_list>
#include <memory>
#include <stdint.h>
#include <vector>
// #include <variant>

namespace sog {
	// using value = std::variant<long long,string,double>;
	using Value = std::experimental::string_view;
	
	struct Source;
	struct Message;
	
	void _prepare(Source *s);
	void _submit(Message m);
	
	struct SinkData {
		virtual ~SinkData() {};
	};
	
	struct Source final {
		std::experimental::string_view file;
		std::experimental::string_view function;
		uint32_t line;
		std::experimental::string_view msg_template;
		std::unique_ptr<SinkData> sink_data;
		size_t value_count;
		const std::experimental::string_view *keys;
		
		Source() {}
		
		inline constexpr Source(
			std::experimental::string_view file,
			std::experimental::string_view function,
			uint32_t line,
			const char *msg_template,
			size_t value_count,
			const std::experimental::string_view *keys
		):
			file(file),
			function(function),
			line(line),
			msg_template(msg_template),
			value_count(value_count),
			keys(keys)
		{
			_prepare(this);
		}
	};
	
	struct Message {
		const Source *source;
		const Value *values;
		
		Message() {}
		constexpr Message(
			const Source *source,
			const Value *values):
			source(source), values(values) {}
		constexpr Message(
			const Source *source,
			std::initializer_list<Value> vals):
			source(source), values(&*vals.begin()) {}
	};
	
	struct Sink {
		virtual ~Sink() {};
		virtual void prepare(Source *source) {};
		virtual void log(Message msg) = 0;
	};
}

#endif

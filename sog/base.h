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
	
	struct SinkData {
		virtual ~SinkData() {};
	};
	
	SinkData *_prepare(const Source *s);
	void _submit(SinkData *s, Message m);
	
	struct Source final {
		std::experimental::string_view file;
		std::experimental::string_view function;
		uint32_t line;
		std::experimental::string_view msg_template;
		size_t value_count;
		const std::experimental::string_view *keys;
		
		Source() {}
		
		inline constexpr Source(
			std::experimental::string_view file,
			std::experimental::string_view function,
			uint32_t line,
			std::experimental::string_view msg_template,
			size_t value_count,
			const std::experimental::string_view *keys
		):
			file(file),
			function(function),
			line(line),
			msg_template(msg_template),
			value_count(value_count),
			keys(keys) { }
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
}

#endif

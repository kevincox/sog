#ifndef SOG___BASE_H
#define SOG___BASE_H

#include <initializer_list>
#include <memory>
#include <vector>
#include <assert.h>
// #include <variant>
#include <experimental/string_view>

namespace sog {
	// using value = std::variant<long long,string,double>;
	using Value = std::string;
	
	struct Source;
	struct Message;
	
	void _prepare(Source *s);
	void _submit(Message m);
	
	struct SinkData {
		virtual ~SinkData() {};
	};
	
	struct Source final {
		const char *file;
		const char *function;
		uint32_t line;
		const char *msg_template;
		std::unique_ptr<SinkData> sink_data;
		size_t value_count;
		const char *const *keys;
		
		Source() {}
		
		inline constexpr Source(
			const char *file,
			const char *function,
			uint32_t line,
			const char *msg_template,
			size_t value_count,
			const char *const *keys
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
		Value *values;
		
		Message() {}
		constexpr Message(const Source *source, Value *values):
			source(source), values(values) {}
	};
	
	struct Sink {
		virtual ~Sink() {};
		virtual void prepare(Source *source) {};
		virtual void log(Message msg) = 0;
	};
	
	struct Watch: public Sink {
		virtual ~Watch() {};
		virtual void watch(const Message &msg) = 0;
		
		virtual void log(const Message &msg) { return watch(msg); };
		void log(Message msg) override { return watch(msg); };
	};
	
	std::unique_ptr<Watch> StdoutWatch();
	
	void init(std::experimental::string_view name, std::unique_ptr<Sink> sink={});
}

#endif

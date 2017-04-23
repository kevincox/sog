#ifndef SOG_BASE_H
#define SOG_BASE_H

#include <assert.h>
#include <boost/variant.hpp>
#include <experimental/string_view>
#include <initializer_list>
#include <memory>
#include <stdint.h>
#include <vector>
// #include <variant>

namespace sog {
	struct Value {
		using Data = boost::variant<
			long,
			double,
			std::string,
			std::experimental::string_view>;
		Data data;
		
		inline Value(double f): data(f) {}
		inline Value(std::string &&s): data(s) {}
		inline Value(std::experimental::string_view s): data(s) {}
		
		template <class T, typename=std::enable_if_t<std::is_integral<T>::value>>
		inline Value(T i): data(static_cast<long>(i)) {}
		
		template <class... A>
		inline Value(A&... a): data(std::experimental::string_view{std::forward<A>(a)...}) {}
		
		// Prevent accidental copies.
		Value(const Value &v): data(v.data) {
			assert(!boost::get<std::string>(&v.data));
		}
		Value(Value &&source) = default;
		
		// Explicit reference.
		Value ref() const { return *this; }
		
		// Explicit copy.
		Value owned() const {
			const std::string *s = boost::get<std::string>(&data);
			if (s)
				return std::string{*s};
			return *this;
		}
		
		bool operator==(const Value &that) const;
	};
	
	
	struct Source;
	struct Message;
	
	using Level = uint8_t;
	namespace level {
		constexpr Level FATAL = 0;
		constexpr Level ERROR = 1;
		constexpr Level WARN = 2;
		constexpr Level INFO = 3;
		constexpr Level DEBUG = 4;
		constexpr Level TRACE = 5;
		
#ifdef NDEBUG
		constexpr Level DFATAL = FATAL;
#else
		constexpr Level DFATAL = ERROR;
#endif
	};
	
	struct SinkData {
		virtual ~SinkData();
	};
	
	/** A log source.
	 * 
	 * A source is a collection of metadata about a "type" of log message. This
	 * is generally created by the LOG() macro. Sources should be immutable once
	 * created.
	 */
	struct Source final {
		Level level = level::ERROR;
		std::experimental::string_view file;
		std::experimental::string_view function;
		uint32_t line = 0;
		std::experimental::string_view msg_template;
		
		/** Number of values for this source.
		 */
		size_t value_count;
		
		/** The keys for this source.
		 * 
		 * This points to the first element of an array of length value_count.
		 */
		const std::experimental::string_view *keys;
		
		Source() {}
		
		inline constexpr Source(
			Level level,
			std::experimental::string_view file,
			std::experimental::string_view function,
			uint32_t line,
			std::experimental::string_view msg_template,
			size_t value_count,
			const std::experimental::string_view *keys
		):
			level(level),
			file(file),
			function(function),
			line(line),
			msg_template(msg_template),
			value_count(value_count),
			keys(keys) { }
	};
	
	/** A logged message.
	 */
	struct Message final {
		/** The source that logged this message.
		 */
		const Source *source;
		
		/** Values of this message.
		 * 
		 * This points to the first value of an array of length
		 * source->value_count. The values map to the respective key in
		 * source->keys.
		 */
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
	
	std::unique_ptr<SinkData> _prepare(const Source *s);
	void _submit(SinkData *d, Message m);
}

#endif

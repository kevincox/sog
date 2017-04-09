#ifndef SOG___MACRO_H
#define SOG___MACRO_H

#define BOOST_PP_VARIADICS 1

#include <boost/preprocessor/arithmetic/mod.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#include "base.h"

namespace sog {
	namespace {
		constexpr
		bool _eq(std::experimental::string_view a, std::experimental::string_view b) {
			if (a.size() != b.size())
				return false;
			for (size_t i = 0; i < a.size(); ++i)
				if (a[i] != b[i])
					return false;
			return true;
		};
		
		constexpr static bool _is_valid(const Source *source) {
			auto t = source->msg_template;
			size_t i = 0;
			
			while (i < t.size()) {
				if (i >= t.size() || t[i] != '$') {
					i++;
					continue;
				}
				
				// Trailing $ is illegal.
				if (++i >= t.size())
					return false;
				
				if (t[i] == '$') {
					i++;
					continue;
				}
				
				auto e = i;
				while (e < t.size() && (
					( 'a' <= t[e] && t[e] <= 'z' ) ||
					( 'A' <= t[e] && t[e] <= 'Z' ) ||
					( '0' <= t[e] && t[e] <= '9' )))
					e++;
				auto key = t.substr(i, e-i);
				
				// Check that the format key has a match.
				size_t ki = 0;
				for (ki = 0; ki < source->value_count; ki++)
					if (_eq(key, source->keys[ki]))
						break;
				if (ki >= source->value_count)
					return false;
			}
			
			return true;
		}
		
		template <typename T>
		struct _valref {
			T &source;
			Value value;
			
			_valref(T &ref): source(ref), value(source) {}
		};
	}
}

#define LOG(level, msg, ...) \
	_sog_LOG(level, msg, \
		BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)/2, \
		BOOST_PP_SEQ_FOR_EACH_I(_sog_KEYS, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)), \
		BOOST_PP_SEQ_FOR_EACH_I(_sog_VALS, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))

#define _sog_LOG(level, msg, n, keys, vals) \
	do { \
		static constexpr std::experimental::string_view _sog_keys[] { keys }; \
		static_assert( \
			n == 0 || sizeof(_sog_keys)/sizeof(_sog_keys[0]) == n, \
			"Odd number of arguments to LOG."); \
		static constexpr ::sog::Source _sog_source { \
			{ __FILE__, sizeof(__FILE__) - 1 }, \
			{ __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__) - 1 }, \
			__LINE__, \
			{ msg, sizeof(msg) - 1 }, \
			n, \
			_sog_keys \
		}; \
		static_assert(::sog::_is_valid(&_sog_source), \
			"Invalid template string (check formatting and argument names.)"); \
		static ::sog::SinkData *_sog_sink_data = ::sog::_prepare(&_sog_source); \
		::sog::_submit(_sog_sink_data, ::sog::Message(&_sog_source, { vals })); \
	} while(false)

#define _sog_KEYS(r, _, i, e, ...) \
	BOOST_PP_IIF(BOOST_PP_MOD(i, 2), /* nothing */, \
		std::experimental::string_view( \
			BOOST_PP_STRINGIZE(e), sizeof(BOOST_PP_STRINGIZE(e))-1)) \
	BOOST_PP_COMMA_IF(BOOST_PP_MOD(i, 2))

#define _sog_VALS(r, _, i, e, ...) \
	BOOST_PP_IIF(BOOST_PP_MOD(i, 2), (e), /* nothing */) \
	BOOST_PP_COMMA_IF(BOOST_PP_MOD(i, 2))

#endif

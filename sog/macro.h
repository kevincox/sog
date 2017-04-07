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
		constexpr size_t _macro_key_len(const char *arg) {
			size_t r = 0;
			while (true) {
				char c = arg[r++];
				if (
					('a' <= c && c <= 'z') ||
					('A' <= c && c <= 'Z') ||
					('0' <= c && c <= '9') ||
					'_' == c) continue;
				return r-1;
			}
		}
		
		template<size_t n>
		struct _StaticStr {
			char str[n+1];
			static constexpr size_t len = n;
			constexpr _StaticStr(const char *arg): str() {
				size_t i = 0;
				while (i < n) {
					str[i] = arg[i];
					i++;
				}
				str[i] = 0;
			}
		};
		
		struct _macro_extract {
			const char *k;
			Value v;
			
			explicit inline _macro_extract(const char *k): k(k) {}
			inline _macro_extract &operator *(Value v) { this->v = v; return *this; }
			inline _macro_extract &operator =(Value v) { return *this; }
			inline _macro_extract &operator =(_macro_extract &&v) = default;
		};
		
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

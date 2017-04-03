#ifndef SOG___MACRO_H
#define SOG___MACRO_H

#define BOOST_PP_VARIADICS 1

#include "boost/preprocessor/seq/for_each_i.hpp"
#include "boost/preprocessor/variadic/to_seq.hpp"

#include "sog/base.h"

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
	}
}

#define LOG(level, msg, ...) \
	_sog_LOG(level, msg, \
		BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), \
		BOOST_PP_SEQ_FOR_EACH_I(_sog_KEYS, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)), \
		BOOST_PP_SEQ_FOR_EACH_I(_sog_KEY_REFS, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)), \
		BOOST_PP_SEQ_FOR_EACH_I(_sog_VALS, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)), \
		BOOST_PP_SEQ_FOR_EACH_I(_sog_VAL_REFS, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))

#define _sog_LOG(level, msg, nn, keys, key_refs, vals, val_refs) \
	do { \
		keys \
		static constexpr std::experimental::string_view _sog_keys[nn] { key_refs }; \
		static ::sog::Source _sog_source { \
			__FILE__, \
			__PRETTY_FUNCTION__, \
			__LINE__, \
			msg, \
			nn, \
			_sog_keys \
		}; \
		vals \
		::sog::Value _sog_args[nn] { val_refs }; \
		::sog::_submit(::sog::Message(&_sog_source, _sog_args)); \
	} while(false)

#define _sog_KEYS(r, _, i, e) \
	_sog_KEYS2(i, e)

#define _sog_KEYS2(i, e) \
	constexpr static auto _sog_key_##i = \
		::sog::_StaticStr<::sog::_macro_key_len(#e)>(#e);

#define _sog_KEY_REFS(r, _, i, e) \
	::std::experimental::string_view(_sog_key_##i.str, _sog_key_##i.len),

#define _sog_VALS(r, _, i, e) \
	_sog_VALS2(e)

#define _sog_VALS2(e) \
	auto _sog_var##e;

#define _sog_VAL_REFS(r, _, i, e) \
	_sog_VAL_REFS2(e)

#define _sog_VAL_REFS2(e) \
	(::sog::_macro_extract(#e) * _sog_var##e).v,

#endif

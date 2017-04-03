#ifndef SOG___MACRO_H
#define SOG___MACRO_H

#define BOOST_PP_VARIADICS 1

#include "boost/preprocessor/stringize.hpp"
#include "boost/preprocessor/variadic/size.hpp"
#include "boost/preprocessor/seq/for_each_i.hpp"
#include "boost/preprocessor/variadic/to_seq.hpp"
#include "boost/preprocessor/facilities/expand.hpp"
#include "boost/preprocessor/control/iif.hpp"
#include "boost/preprocessor/control/if.hpp"
#include "boost/preprocessor/arithmetic/mod.hpp"
#include "boost/preprocessor/punctuation/comma.hpp"
#include "boost/preprocessor/punctuation/comma_if.hpp"
#include "boost/preprocessor/facilities/identity.hpp"
#include "sog/base.h"

#define _sog_CALL(x) x()

#define LOG(level, msg, ...) \
	_sog_LOG(level, msg, \
		BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), \
		BOOST_PP_SEQ_FOR_EACH_I(_sog_KEYS, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)), \
		BOOST_PP_SEQ_FOR_EACH_I(_sog_VALS, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))

#define _sog_LOG(level, msg, nn, keys, vals) \
	do { \
		constexpr size_t _sog_n = nn/2; \
		static constexpr const char* _sog_keys[_sog_n] { keys }; \
		static ::sog::Source _sog_source { \
			__FILE__, \
			__PRETTY_FUNCTION__, \
			__LINE__, \
			msg, \
			_sog_n, \
			_sog_keys \
		}; \
		::sog::Value _sog_args[_sog_n] { vals }; \
		::sog::_submit(::sog::Message(&_sog_source, _sog_args)); \
	} while(false)

#define _sog_KEYS(r, _, i, e, ...) \
	BOOST_PP_IIF(BOOST_PP_MOD(i, 2), /* nothing */, BOOST_PP_STRINGIZE(e)) \
	BOOST_PP_COMMA_IF(BOOST_PP_MOD(i, 2))

#define _sog_VALS(r, _, i, e, ...) \
	BOOST_PP_IIF(BOOST_PP_MOD(i, 2), (e), /* nothing */) \
	BOOST_PP_COMMA_IF(BOOST_PP_MOD(i, 2))

#endif

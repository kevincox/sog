#ifndef SOG_MACRO_H
#define SOG_MACRO_H

#define BOOST_PP_VARIADICS 1

#include <boost/preprocessor/arithmetic/mod.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#include "base.h"

#ifndef _sog_LINE
#define _sog_LINE __LINE__
#endif

namespace sog {
	namespace {
		constexpr
		bool _eq(std::string_view a, std::string_view b) {
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
			
#if defined(__GNUC__) && !defined(__clang__)
			// gcc hangs when running this function.
			// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81042
			return true;
#pragma message "Not checking message formats to prevent hanging GCC."
#endif
			
			while (i < t.size()) {
				if (i >= t.size() || t[i] != '$') {
					i++;
					continue;
				}
				
				// Trailing $ is illegal.
				if (++i >= t.size())
					throw "Template ends with '$'";
				
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
					throw "Unknown parameter in template.";
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

/** Log a message.
 * 
 * Note that none, some or all values may be evaluated depending on if the log
 * message is enabled and what values the sink needs. (currently all values
 * are always evaluated but this will change)
 * 
 * @param level The severity of the message as a sog::level constant or a number.
 * @param msg The human readable message.
 * @param ... Arguments in pairs of keys (unquoted identifiers) and values (any c++ expression).
 */
#define LOG(level, msg, ...) \
	_sog_LOG(level, msg, \
		BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)/2, \
		BOOST_PP_SEQ_FOR_EACH_I(_sog_KEYS, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)), \
		BOOST_PP_SEQ_FOR_EACH_I(_sog_VALS, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))

#define _sog_LOG(lvl, msg, n, keys, vals) \
	do { \
		using namespace ::sog::level; \
		static constexpr std::string_view _sog_keys[] { keys }; \
		static_assert( \
			n == 0 || sizeof(_sog_keys)/sizeof(_sog_keys[0]) == n, \
			"Odd number of arguments to LOG."); \
		static constexpr ::sog::Source _sog_source { \
			lvl, \
			{ __FILE__, sizeof(__FILE__) - 1 }, \
			{ __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__) - 1 }, \
			_sog_LINE, \
			{ msg, sizeof(msg) - 1 }, \
			n, \
			_sog_keys \
		}; \
		static_assert(::sog::_is_valid(&_sog_source), \
			"Invalid template string (check formatting and argument names.)"); \
		static sog::Prepared _sog_prepared = ::sog::_prepare(&_sog_source); \
		if (!_sog_prepared.log) \
			break; \
		::sog::_submit(_sog_prepared.sink_data.get(), ::sog::Message(&_sog_source, { vals })); \
	} while(false)

#define _sog_KEYS(r, _, i, e, ...) \
	BOOST_PP_IIF(BOOST_PP_MOD(i, 2), /* nothing */, \
		std::string_view( \
			BOOST_PP_STRINGIZE(e), sizeof(BOOST_PP_STRINGIZE(e))-1)) \
	BOOST_PP_COMMA_IF(BOOST_PP_MOD(i, 2))

#define _sog_VALS(r, _, i, e, ...) \
	BOOST_PP_IIF(BOOST_PP_MOD(i, 2), (e), /* nothing */) \
	BOOST_PP_COMMA_IF(BOOST_PP_MOD(i, 2))

#endif

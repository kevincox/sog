#ifndef SOG_C_H
#define SOG_C_H

#include <stdint.h>
#include <stdlib.h>

// A sequence of UTF-8 bytes.
struct SogStr {
	const char *data;
	size_t size;
};

enum SogValueType { SOG_LONG, SOG_DOUBLE, SOG_STR };

struct SogValue {
	enum SogValueType type;
	union {
		long val_long;
		double val_double;
		struct SogStr val_str;
	};
};

struct SogSource;

void sog_init_pretty(void);

// Make a new source. All things referenced by must stay alive until the
// returned source is deleted.
struct SogSource *sog_source_new(
	uint16_t level,
	struct SogStr file,
	struct SogStr function,
	uint32_t line,
	struct SogStr msg_template,
	size_t value_count,
	struct SogStr *keys);

void sog_source_delete(struct SogSource *source);

void sog_submit(struct SogSource *source, const struct SogValue *values);

#endif

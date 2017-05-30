#include <gtest/gtest.h>

int logger; // Hack

extern "C" {

#include "sog/c.h"

void do_c_test(void) {
	sog_init_pretty();
	
	SogStr keys[] = {
		{"vala", 4},
		{"b", 1},
		{"str", 3},
	};
	
	SogSource *source = sog_source_new(
		2,
		{"src/my_file.c", 13},
		{"do_c_test", 8},
		78,
		{"my msg $vala $b $str", 20},
		3,
		keys);
	
	SogValue vals[3];
	vals[0].type = SOG_LONG;
	vals[0].val_long = 4;
	vals[1].type = SOG_DOUBLE;
	vals[1].val_double = 5.23;
	vals[2].type = SOG_STR;
	vals[2].val_str = {"log val", 7};
	
	sog_submit(source, vals);
	
	sog_source_delete(source);
	
	source = sog_source_new(
		0,
		{"src/other_file.c", 16},
		{"my_c_test", 8},
		90,
		{"my msg $vala $b $str", 20},
		3,
		keys);
	
	vals[0].val_long = 8;
	vals[1].val_double = 1.11;
	vals[2].val_str = {"foo", 3};
	sog_submit(source, vals);
}

}

TEST(Sog, C) {
	EXPECT_DEATH(do_c_test(),
		"2000-02-13T15:30:00 W my_file.c:78 my msg 4 5.23 log val\n"
		"2000-02-13T15:30:00 F other_file.c:90 my msg 8 1.11 foo\n");
}

#include "sog/init.h"
#include "sog/journald.h"
#include "sog/macro.h"
#include "sog/pretty.h"

int main(void) {
	sog::JournaldSink sink;
	sog::init(&sink);
	
	LOG(INFO, "My first log.");
	LOG(INFO, "Log with unused data.",
		int, 4,
		double, 4.23,
		str, "A string");
	
	return 0;
}

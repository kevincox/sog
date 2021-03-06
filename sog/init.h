#ifndef SOG_INIT_H
#define SOG_INIT_H

#include "sink.h"

namespace sog {
	/** Initalize sog.
	 * 
	 * It is an error to log a message before init is called.
	 * 
	 * It is an error to call this method twice.
	 * 
	 * @param sink The sink to send logs to. If null a sog::PrettySink to stderr
	 *	will be used.
	 */
	void init(Sink *sink=nullptr);
}

#endif

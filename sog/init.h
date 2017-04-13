#ifndef SOG___INIT_H
#define SOG___INIT_H

#include "sink.h"

namespace sog {
	/** Initalize sog.
	 * 
	 * It is an error to log a message before init is called.
	 * 
	 * It is an error to call this method twice.
	 * 
	 * @param The sink to send logs to. If null a sog::PrettySink to stderr
	 *	will be used.
	 */
	void init(Sink *sink=nullptr);
}

#endif

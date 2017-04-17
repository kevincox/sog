# Sog

Warning: sog is still a work in progress, but the basics work.

Sog is an ad-hoc structured logging library for c++. It focuses on ease of use and performance.

```c++
LOG(INFO, "I'm writing a $type log message with $lib!",
	type, "structured",
	lib, "sog");
```

## Goals

### Ad-hoc

Ad-hoc means that there is no predefined structure. Sog isn't aimed at logging events of a specific schema. While it can be used for this in many cases it lacks features such as multiple streams that make it non-idea.

### Efficient

Using c++ features sog manages to make logging very cheep. As much as possible is put into a once-per message setup, allowing subsequent messages to do only the following.

- Check source has been initialized. (c++ local static check)
- TODO: Check if source is enabled. (currently all sources are enabled).
- Prepare message values.
- Virtual function call to sink.

This requires no allocations once prepared. This overhead is likely far less then any useful source (and often cheaper then computing the values to log).

### Flexible

Sog uses pluggable sinks. This means that your program can choose what to do with it's log messages. It's easy to [write your own](sog/sink.h) and sog comes with the following sinks created for you.
- PrettySink: This sink it intended for human output, it evaluates the template with the provided values and logs the message to the terminal (or any provided std::ostream).
- JsonSink: This logs one JSON object on it's own line for each message.
- TODO: FanSink: This sink sends messages to any number of other sinks. Useful for logging full messages (example JSON) while still outputting to the terminal for developers.
- TODO: BufferSink: This sink buffers messages temporarily in memory before forwarding to another sink.

## Getting Started

### Initialize Sog

Initialize sog at the start of main. It is an error to initialize sog twice or to log a message before sog is initialized.

```c++
#include <sog/init.h>

sog::init();
```

By default sog uses the PrettySink to stderr. You can pass a sink to `init()` to use a different one.

```c++
#include <sog/init.h>
#include <sog/json.h> // Coming soon.

sog::JsonSink sink;
sog::init(&sink);
```

### Log Away

Sog provides a macro to make logging easy. It takes care of stringifying keys as well as making sure the [source](docs/glossary.md#source) is prepared the first time this log is hit.

```
LOG(level, human_message, [key, value]...);
```

```c++
#include <sog/macro.h>

auto host = "example.com";
net::ip addr = "2001:db8::99";
LOG(INFO, "Connecting to $host ($ip)",
	host, host,
	ip, addr,
	retries, 3);
```

## Documentation

General documentation is available in [the docs subdirectory](docs/) and c++ interface documentation is available in the headers. Some useful files are:
- [sog/init.h](sog/init.h) contains useful information for initializing sog.
- [sog/macro.h](sog/macro.h) contains the LOG macro.
- [sog/sink.h](sog/sink.h) describes how to write your own sink.
- [sog/base.h](sog/base.h) contains the base data types.

## Build

Sog depends on the following libraries.
- boost: Build dependency for sog and any libraries using sog.
- googletest: Test dependency for sog.

Sog can be build with [bazel](https://bazel.build).

```sh
$ bazel build -c opt :sog
```

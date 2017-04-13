# Sog Glossary

## Message

A message is a collection of key/value pairs, as well as some metadata (file, line ...) that is logged. A message always has an associated [source](#source).

## Sink

A sink is where [messages](#message) go. Sog has some sink types built in or you can add your own.

## Source

A source is a thing that sends a single type of log. Typically this maps to a `LOG` macro in your code.

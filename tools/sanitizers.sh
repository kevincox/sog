#!/bin/bash

export CC=clang++

cd "$(realpath "${BASH_SOURCE[0]}/../..")"

status=0

for opt in fastbuild opt; do
	# Note: memory sanitizer can't be used as it requires a recompile of all
	# linked libraries.
	for san in address thread undefined leak; do
		echo "Opt: $opt, San: $san"
		bazel test \
			--bazelrc=tools/bazel.rc \
			-c $opt --copt -fsanitize=$san --linkopt -fsanitize=$san ...
		[[ $? == 0 ]] || status=1
	done
done

exit $status

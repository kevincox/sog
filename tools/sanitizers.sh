#!/bin/bash

export CC=clang++

cd "$(realpath "${BASH_SOURCE[0]}/../..")"

status=0

for opt in fastbuild opt; do
	for san in address thread memory undefined leak; do
		echo "Opt: $opt, San: $san"
		bazel test \
			--bazelrc=tools/bazel.rc \
			-c $opt --copt -fsanitize=$san --linkopt -fsanitize=$san ...
		r=$?
		[[ $r == 0 ]] || status=1
	done
done

exit $status

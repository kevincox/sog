#!/bin/bash


cd "$(realpath "${BASH_SOURCE[0]}/../..")"

status=0

for cc in clang++ g++; do
	for opt in fastbuild opt; do
		# Note: memory sanitizer can't be used as it requires a recompile of all
		# linked libraries.
		for san in address thread undefined leak; do
			echo "Opt: $opt, San: $san"
			CC="$cc" bazel test \
				-c $opt --copt -fsanitize=$san --linkopt -fsanitize=$san ...
			[[ $? == 0 ]] || status=1
		done
	done
done

exit $status

#!/bin/bash

set -au

cd "$(realpath "$(dirname ${BASH_SOURCE[0]})/..")"

status=0

for cc in clang gcc; do
	args=(qbs --build-directory build -p autotest-runner profile:$cc)
	for opt in fastbuild opt; do
		# Note: memory sanitizer can't be used as it requires a recompile of all
		# linked libraries.
		for san in address thread undefined leak; do
			args+=(sanitize-$cc-$opt-$san cpp.driverFlags:-fsanitize=$san)
		done
	done
	
	echo ${args[@]}
	"${args[@]}"
	[[ $? == 0 ]] || status=1
done

exit $status

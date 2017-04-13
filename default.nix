with import <nixpkgs> {};

stdenv.mkDerivation rec {
	name = "sog";
	meta = {
		description = "Structured logging library.";
		homepage = https://github.com/kevincox/sog;
	};
	
	outputs = [ "dev" "lib" "out" ];
	
	src = builtins.filterSource (name: type:
		(lib.hasPrefix (toString ./BUILD) name) ||
		(lib.hasPrefix (toString ./sog) name) ||
		(lib.hasPrefix (toString ./test) name) ||
		(lib.hasPrefix (toString ./tools) name) ||
		(lib.hasPrefix (toString ./WORKSPACE) name)) ./.;
	
	buildInputs = with pkgs; [ bazel ];
	
	bazelCmd = lib.concatStringsSep " " [
		"bazel"
		"--output_base=/tmp"
		"--bazelrc=tools/bazel.rc"
		"--max_idle_secs=10"
	];
	CPLUS_INCLUDE_PATH = lib.concatStringsSep ":" [
		"${pkgs.boost.dev}/include/"
		"${pkgs.gtest}/include/"
	];
	
	doCheck = true;
	checkPhase = ''
		$bazelCmd test ...
	'';
	
	buildPhase = ''
		export HOME=$TMPDIR # Trick bazel into a better cache location.
		$bazelCmd build -c opt :sog
	'';
	
	installPhase = ''
		install -Dt "$lib/lib/" bazel-bin/libsog.so
		install -Dt "$dev/include/" sog/*.h
		
		touch $out
	'';
}

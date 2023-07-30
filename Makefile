.PHONY: default
default:
	cmake -S. -B Debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cmake --build Debug
	cp Debug/compile_commands.json .

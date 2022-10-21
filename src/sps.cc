#include "sps.hh"

#include <stdio.h>

extern "C" {
	#include "ruby.h"
}

/**
 * @brief SpaceshipCmdArgs is a wrapper for the command line arguments.
 * By using that, we can pass argument data to functions much easier.
 */
struct SpaceshipCmdArgs {
	int argc;
	char **argv;
};

void sps_fatal_error(char* error_tag, char* cause) {
    printf("ERROR: [%s] %s\n", error_tag, cause);
    printf("This program cannot continue running.\n");
    exit(EXIT_FAILURE);
}

/**
 * @brief "sps_get_args" is responsible for parsing command line arguments.
 * This command handles the parsing and returns:
 * - 0 (There is no config argument.) -> Continue
 * - 1 (There are some config arguments and they use SPS processor.) -> Continue
 * - 2 (There are some config arguments and they do not use the SPS processor.) -> Halt
 * @param args SpaceshipCmdArgs object that contains arguments.
 * @return int The result code.
 */
int sps_get_args(struct SpaceshipCmdArgs args) {
	
	for (int i=0; i<args.argc; i++) {
		if (args.argv[i][0] == '-') {
			switch (args.argv[i][1]) {
			case 'v':
				printf("GetArgs version 1.0\n");
				return SPS_COMMAND_CONFIG_WO_SPS;
			};
		}
	}

	return SPS_COMMAND_SUCCESSFUL;
}


/*
	The main method of the program.
*/
int main(int argc, char** argv) {
	struct SpaceshipCmdArgs args = {argc, argv};

	if (sps_get_args(args) == 2)
	{
		return -1;
	}

	printf("Hello Spaceship and hello World!\n");
	
    if (ruby_setup()) {
        sps_fatal_error("RUBYVM", "Ruby VM initialize failed.");
    }
	
	char *options[] = { "-v", "-eputs 'Hello, Spaceship from Ruby'"}; 
	void *node = ruby_options(2, options);

	int state;
	if (ruby_executable_node(node, &state))
	{
		state = ruby_exec_node(node);
	}

	if (state)
	{
		printf("There was an exception from Ruby VM.\n");
	}
	return ruby_cleanup(0);
}

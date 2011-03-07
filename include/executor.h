/**
 * @file
 * Command Shell Executor module header.
 *
 * The Executor module is used to run an OS command with reduced security risk.
 *
 * <p>Implementations should do as much as possible to minimize the risk of
 * injection into either the command or parameters. In addition, implementations
 * should timeout after a specified time period in order to help prevent denial
 * of service attacks.</p>
 *
 * <p>The functions should perform logging and error handling as
 * well. Finally, an implementation should handle errors and generate an
 * log with all the necessary information.</p>
 *
 * <p>This reference implementation does all of the above except for the timeout.</p>
 *
 * @since June 1, 2007
 */

#include <stdio.h>
#include <stdbool.h>

#ifndef _EXECUTOR_H_
#define _EXECUTOR_H_

#include "codec.h"

/**
 * Spawns a new process to execute the given command.
 *
 * @param executable The executable filename to be executed.
 * @param params An array of command-line parameters
 * @param paramcount The number of parameters
 * @param workdir The working directory for the new process
 * @param c A codec to be used to encode the command string before passing it to the
 * 		command shell interpreter.
 * @param logParams 1 if the parameter values are to be logged, 0 if not.
 */
extern char *esapi_execute(char *executable, char **params, int paramcount,
		char *workdir, codec *c, bool logParams);

#endif /* _EXECUTOR_H_ */

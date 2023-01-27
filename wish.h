#ifndef WISH_H
#define WISH_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 256
#define MAX_ARGS 32

/**
 *Runs a batch file containing a list of commands.
 *
 * @param fileName the name of the batch file containing the commands to be
 *executed
 */
void run_batch(char *fileName);

/**
 * Runs the shell in interactive mode, where the user can input commands
 * and receive output in the terminal. Continues running until the user
 * enters "exit".
 */
void run_interactive();

/**
 * Runs a command in the shell using the passed in arguments.
 *
 * @param args An array of arguments that the command takes.
 * @param argc The number of arguments in the array.
 */
void run_command(char **args, int argc);

/**
 * Splits a line into individual arguments
 *
 * @param line The line to be split
 * @param args The array to store the split arguments
 *
 * @return The number of arguments
 */
int split_line(char *line, char **args);

/**
 * Trims leading and trailing whitespaces from the given command.
 *
 * @param command The command to trim.
 */
void trim(char *command);

/**
 * handles path command
 * @param argc number of arguments
 * @param args array of arguments
 *
 */
void path_command(int argc, char **args);

#endif

#ifndef WISH_H
#define WISH_H

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 256
#define MAX_ARGS 32
#define MAX_COMMANDS 32
#define MAX_FILENMAE 256
#define MAX_PATH_LENGTH 2000

/**
 * Runs a batch file containing a list of commands.
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
 * @param redirection determines whether output should be printed in the
 * terminal or to g_outputFile
 */
void run_command(char **args, int argc, int redirection);

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
 * updates the current search path for the shell, If no arguments are passed,
 * the current search path is cleared. If arguments are passed, they are
 * concatenated with ':' separator and used as the new search path.
 *
 * @param argc number of arguments
 * @param args array of arguments
 *
 */
void path_command(int argc, char **args);

/**
 * Splits a command string by the '&' delimiter and stores the resulting
 * commands in an array.
 *
 * @param command The command string to split.
 * @param commands The array to store the resulting commands in.
 *
 * @return The number of commands found in the command string.
 */
int split_commands(char *command, char **commands);

/**
 * checks if a redirection symbol ">" is present in the line
 * and separates the command from the filename
 *
 * @param line: the command line to be checked for redirection
 *
 * @return:
 *  -1: no ">" found
 *  -2: error, more than one ">" found
 *  -3: error, more than one file name after ">"
 *  -4: error using ">" without pass a file name
 *  -1<: success, index of ">" found
 */
int check_redirection(char *line);

/**
 * Handle redirection of output to a file.
 */
void handle_redirection();


#endif

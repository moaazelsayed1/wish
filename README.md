# Unix Shell

This is a custom implementation of a shell in C, created for educational purposes.
It supports basic shell functionality such as running commands, parallel commands, redirecting output, and setting the search path.

## Specifications
This shell is built as a project in the OSTEP course. You can find the project prompt and instructions [here](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/processes-shell).

## Building and Running

1. Build the project
```
make
```
2. Run the shell in interactive mode
```
./wish
```
3. Run the shell in batch mode
```
./wish batchfile.txt
```

## Function documentation

Each function is documented in the header file using the Google-style documentation format, for example:
```
/**
 * Runs a batch file containing a list of commands.
 *
 * @param fileName the name of the batch file containing the commands to be
 *executed
 */
```




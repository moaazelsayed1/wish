#include "wish.h"

char g_errorMessage[32] = "An error has occurred\n";
char *g_args[MAX_ARGS];

int main(int argc, char *argv[]) {
  if (argc == 1) {
    run_interactive();
  } else if (argc == 2) {
    run_batch(argv[1]);
  } else if (argc > 2) {
    write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
    return 1;
  }
  return 0;
}

void run_batch(char *fileName) {
  FILE *file = fopen(fileName, "r");
  if (file == NULL) {
    write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
    exit(EXIT_FAILURE);
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, file)) != -1) {
    if (strlen(line) == 0 || strspn(line, " \t\r\n\f\v") == strlen(line))
      continue;

    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }

    trim(line);
    int argc = split_line(line, g_args);
    run_command(g_args, argc);
  }

  fclose(file);
  if (line)
    free(line);
}
void run_interactive() {}

void run_command(char **args, int argc) {
  if (strcmp(args[0], "exit") == 0) {
    if (argc != 1) {
      write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
    } else {
      exit(0);
    }
  } else if (strcmp(args[0], "path") == 0) {
    path_command(argc, args);
  } else if (strcmp(args[0], "cd") == 0) {
    if (args[1] == NULL) {
      write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
    } else {
      if (chdir(args[1]) != 0) {
        write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
      }
    }
  }

  pid_t child_pid = fork();
  int status;
  if (child_pid < 0) {
    // fork failed
    write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
    return;
  } else if (child_pid == 0) {
    // child process
    if (execvp(args[0], args) < 0) {
    }
  } else {
    // parent process
    waitpid(child_pid, &status, 0);
  }
}

void path_command(int argc, char **args) {}

void trim(char *command) {
  int len = strlen(command);

  int start = 0;
  int end = len - 1;

  while (start < len && isspace(command[start])) {
    start++;
  }
  while (end > start && isspace(command[end])) {
    end--;
  }
  int i;
  for (i = 0; i <= end - start; i++) {
    command[i] = command[start + i];
  }

  command[i] = '\0';
}

int split_line(char *line, char **args) {
  int argc = 0;
  char *token;

  token = strtok(line, " ");
  while (token != NULL) {
    args[argc++] = token;
    token = strtok(NULL, " ");
  }
  args[argc] = NULL;

  return argc;
}

#include "wish.h"

char g_errorMessage[32] = "An error has occurred\n";
char g_finalCommand[MAX_COMMAND_LENGTH], g_outputFile[MAX_FILENMAE];
char *g_args[MAX_ARGS];
char g_path[MAX_PATH_LENGTH];
pid_t g_pid[MAX_COMMANDS];
int g_numPids = 0;

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
    char *commands[MAX_COMMANDS];
    int num_commands = split_commands(line, commands);

    for (int i = 0; i < num_commands; i++) {
      int redirection = check_redirection(commands[i]);
      if (redirection == -2 || redirection == -3 || redirection == -4) {
        continue;
      }

      trim(g_finalCommand);
      trim(g_outputFile);

      int argc = split_line(commands[i], g_args);
      run_command(g_args, argc, redirection);
    }

    for (int i = 0; i < g_numPids; i++) {
      waitpid(g_pid[i], NULL, 0);
    }
  }

  fclose(file);
  if (line)
    free(line);
}
void run_interactive() {
  char *line;
  size_t len = 0;
  while (1) {
    printf("wish> ");
    getline(&line, &len, stdin);

    int length = strlen(line);
    if (length > 0 && line[length - 1] == '\n') {
      line[length - 1] = '\0';
    }
    trim(line);

    if (strlen(line) == 0) {
      continue;
    }

    char *commands[MAX_COMMANDS];
    int num_commands = split_commands(line, commands);

    for (int i = 0; i < num_commands; i++) {
      int redirection = check_redirection(commands[i]);
      if (redirection == -2 || redirection == -3 || redirection == -4) {
        continue;
      }

      trim(g_finalCommand);
      trim(g_outputFile);
      int argc = split_line(commands[i], g_args);

      run_command(g_args, argc, redirection);
    }
    for (int i = 0; i < g_numPids; i++) {
      waitpid(g_pid[i], NULL, 0);
    }
  }
}

void run_command(char **args, int argc, int redirection) {
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
  int found = 0;
  char *cpath = (char *)malloc(strlen(g_path) + 1);
  strncpy(cpath, g_path, strlen(g_path));
  char *token = strtok(cpath, ":");
  while (token != NULL) {
    char new_path[MAX_PATH_LENGTH];
    if (token[0] != '/') {
      getcwd(new_path, MAX_PATH_LENGTH);
      strcat(new_path, "/");
      strcat(new_path, token);
    } else {
      strcpy(new_path, token);
    }
    sprintf(new_path, "%s/%s", new_path, args[0]);
    if (access(new_path, F_OK) == 0) {
      strncpy(cpath, new_path, strlen(new_path));
      cpath[strlen(new_path)] = '\0';
      found = 1;
      break;
    }
    token = strtok(NULL, ":");
  }
  if (!found) {
    write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
    free(cpath);
    return;
  }

  pid_t child_pid = fork();
  int status;
  if (child_pid < 0) {
    // fork failed
    write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
    return;
  } else if (child_pid == 0) {
    // child process
    if (redirection) {
      handle_redirection();
    }

    if (execvp(args[0], args) < 0) {
    }
  } else {
    // parent process
    waitpid(child_pid, &status, 0);
  }
}

void path_command(int argc, char **args) {
  if (argc == 1) {
    strcpy(g_path, "");
  } else {
    strcpy(g_path, args[1]);
    for (int i = 2; i < argc; i++) {
      strcat(g_path, ":");
      strcat(g_path, args[i]);
    }
  }
}

int check_redirection(char *line) {
  if (line[strlen(line) - 1] == '>') {
    write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
    return -4;
  }
  char *token = strtok(line, ">");
  int count = 0;
  while (token != NULL) {
    if (count == 0) {
      strcpy(g_finalCommand, token);
    } else if (count == 1) {
      if (token[0] == '\0') {
        return -2;
      }
      strcpy(g_outputFile, token);
    } else {
      write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
      return -2;
    }
    count++;
    token = strtok(NULL, ">");
  }
  if (count == 1) {
    return -1;
  } else if (count == 2) {
    char *extra = strtok(g_outputFile, " ");
    if (extra != NULL) {
      extra = strtok(NULL, " ");
      if (extra != NULL) {
        write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
        return -3;
      }
    }
    return count;
  }
  return -2;
}

void handle_redirection() {
  int file = open(g_outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
  if (file < 0) {
    write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
    exit(EXIT_FAILURE);
  }
  dup2(file, STDOUT_FILENO);
  close(file);
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
int split_commands(char *line, char **commands) {
  char *token;
  int i = 0;
  token = strtok(line, "&");
  while (token != NULL) {
    commands[i] = token;
    token = strtok(NULL, "&");
    i++;
  }
  return i;
}
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

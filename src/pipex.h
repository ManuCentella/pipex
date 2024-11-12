#ifndef PIPEX_H
#define PIPEX_H

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include "../libft/inc/libft.h"

void exec_cmd(char *command, char **env);
void execute_child(char **args, int *pipe_fd, char **env);
void execute_parent(char **args, int *pipe_fd, char **env);
int open_file(char *file, int in_or_out);
void error_and_exit(int exit_code);
char *get_path(char *cmd, char **env);
void ft_free_tab(char **tab);
char *my_getenv(char *name, char **env);
int ft_strcmp(const char *s1, const char *s2);
char *join_and_check_access(char *path_part, char *cmd, char **allpath);
void exec(char *cmd, char **env);

#endif
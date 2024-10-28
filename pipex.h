#ifndef PIPEX_H
#define PIPEX_H

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include "libft/inc/libft.h" // Asegúrate de que la ruta es correcta

void exec_cmd(char *cmd, char **env);
void child_process(char **av, int *p_fd, char **env);
void parent_process(char **av, int *p_fd, char **env);
int open_file(char *file, int in_or_out);
void exit_handler(int n_exit);
char *get_path(char *cmd, char **env);
void ft_free_tab(char **tab);
char *my_getenv(char *name, char **env);
int ft_strcmp(const char *s1, const char *s2);

#endif
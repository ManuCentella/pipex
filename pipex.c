#include "pipex.h"

void error_and_exit(int exit_code) {
    if (exit_code == 1) {
        fprintf(stderr, "Usage: ./pipex infile cmd1 cmd2 outfile\n");
    } else if (exit_code == 2) {
        perror("Error");
    }
    exit(exit_code);
}

int ft_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

int open_file(char *file, int flags) {
    int ret = open(file, flags, 0777);
    if (ret == -1) {
        perror("Error al abrir archivo");
        exit(1);
    }
    return ret;
}

void ft_free_tab(char **tab) {
    size_t i = 0;
    while (tab[i]) {
        free(tab[i]);
        i++;
    }
    free(tab);
}

char *my_getenv(char *name, char **env) {
    int i = 0;
    int j;
    char *sub;

    while (env[i]) {
        j = 0;
        while (env[i][j] && env[i][j] != '=')
            j++;
        sub = ft_substr(env[i], 0, j);
        if (ft_strcmp(sub, name) == 0) {
            free(sub);
            return (env[i] + j + 1);
        }
        free(sub);
        i++;
    }
    return NULL;
}

char *get_path(char *cmd, char **env) {
    char **allpath, *path_env, *full_path, *path_part;
    int i = 0;

    if (!(path_env = my_getenv("PATH", env)) || !(allpath = ft_split(path_env, ':')))
        return cmd;

    while (allpath[i]) {
        path_part = ft_strjoin(allpath[i], "/");
        full_path = ft_strjoin(path_part, cmd);
        free(path_part);
        if (access(full_path, F_OK | X_OK) == 0) {
            ft_free_tab(allpath);
            return full_path;
        }
        free(full_path);
        i++;
    }
    ft_free_tab(allpath);
    return cmd;
}

void exec_cmd(char *cmd, char **env) {
    char **s_cmd = ft_split(cmd, ' ');
    char *path = get_path(s_cmd[0], env);
    if (execve(path, s_cmd, env) == -1) {
        fprintf(stderr, "pipex: command not found: %s\n", s_cmd[0]);
        ft_free_tab(s_cmd);
        exit(0);
    }
}

void execute_child(char **args, int *pipe_fd, char **env) {
    int input_fd = open_file(args[1], O_RDONLY);
    dup2(input_fd, STDIN_FILENO);
    dup2(pipe_fd[1], STDOUT_FILENO);
    close(pipe_fd[0]);
    exec_cmd(args[2], env);
}

void execute_parent(char **args, int *pipe_fd, char **env) {
    int output_fd = open_file(args[4], O_WRONLY | O_CREAT | O_TRUNC);
    dup2(output_fd, STDOUT_FILENO);
    dup2(pipe_fd[0], STDIN_FILENO);
    close(pipe_fd[1]);
    exec_cmd(args[3], env);
}

int main(int ac, char **av, char **env) {
    int pipe_fd[2];
    pid_t pid;

    if (ac != 5)
        error_and_exit(1);
    if (pipe(pipe_fd) == -1)
        error_and_exit(2);
    pid = fork();
    if (pid == -1)
        error_and_exit(2);
    if (pid == 0)
        execute_child(av, pipe_fd, env);
    execute_parent(av, pipe_fd, env);
}
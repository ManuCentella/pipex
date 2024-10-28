#include "pipex.h"

void exit_handler(int n_exit) {
    if (n_exit == 1)
        ft_putstr_fd("./pipex infile cmd cmd outfile\n", 2);
    exit(0);
}

int ft_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

int open_file(char *file, int in_or_out) {
    int ret;

    if (in_or_out == 0)
        ret = open(file, O_RDONLY, 0777);
    if (in_or_out == 1)
        ret = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (ret == -1) {
        perror("Error al abrir archivo");
        exit(1);
    }
    return (ret);
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
    int i = -1;
    char *exec;
    char **allpath;
    char *path_part;
    char **s_cmd;

    allpath = ft_split(my_getenv("PATH", env), ':');
    s_cmd = ft_split(cmd, ' ');
    while (allpath[++i]) {
        path_part = ft_strjoin(allpath[i], "/");
        exec = ft_strjoin(path_part, s_cmd[0]);
        free(path_part);
        if (access(exec, F_OK | X_OK) == 0) {
            ft_free_tab(s_cmd);
            ft_free_tab(allpath);
            return exec;
        }
        free(exec);
    }
    ft_free_tab(allpath);
    ft_free_tab(s_cmd);
    return cmd;
}

void exec_cmd(char *cmd, char **env) {
    char **s_cmd;
    char *path;

    s_cmd = ft_split(cmd, ' ');
    path = get_path(s_cmd[0], env);
    if (execve(path, s_cmd, env) == -1) {
        ft_putstr_fd("pipex: command not found: ", 2);
        ft_putendl_fd(s_cmd[0], 2);
        ft_free_tab(s_cmd);
        exit(0);
    }
}

void child_process(char **av, int *p_fd, char **env) {
    int fd;

    fd = open_file(av[1], 0);
    dup2(fd, 0);
    dup2(p_fd[1], 1);
    close(p_fd[0]);
    exec_cmd(av[2], env);
}

void parent_process(char **av, int *p_fd, char **env) {
    int fd;

    fd = open_file(av[4], 1);
    dup2(fd, 1);
    dup2(p_fd[0], 0);
    close(p_fd[1]);
    exec_cmd(av[3], env);
}

int main(int ac, char **av, char **env) {
    int p_fd[2];
    pid_t pid;

    if (ac != 5)
        exit_handler(1);
    if (pipe(p_fd) == -1)
        exit(-1);
    pid = fork();
    if (pid == -1)
        exit(-1);
    if (!pid)
        child_process(av, p_fd, env);
    parent_process(av, p_fd, env);
}
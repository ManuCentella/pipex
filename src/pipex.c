/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcentell <mcentell@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:35:53 by mcentell          #+#    #+#             */
/*   Updated: 2024/11/12 16:39:31 by mcentell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


void exec_cmd(char *command, char **env)
{
    char **command_args;
    char *path;

    command_args = ft_split(command, ' ');
    path = get_path(command_args[0], env);
    if (execve(path, command_args, env) == -1)
    {
        fprintf(stderr, "pipex: command not found: %s\n", command_args[0]);
        ft_free_tab(command_args);
        exit(0);
    }
}

void execute_child(char **args, int *pipe_fd, char **env)
{
    int input_fd;

    input_fd = open_file(args[1], O_RDONLY);
    dup2(input_fd, STDIN_FILENO);
    dup2(pipe_fd[1], STDOUT_FILENO);
    close(pipe_fd[0]);
    exec_cmd(args[2], env);
}

void execute_parent(char **args, int *pipe_fd, char **env)
{
    int output_fd;

    output_fd = open_file(args[4], O_WRONLY | O_CREAT | O_TRUNC);
    dup2(output_fd, STDOUT_FILENO);
    dup2(pipe_fd[0], STDIN_FILENO);
    close(pipe_fd[1]);
    exec_cmd(args[3], env);
}

char	*get_path(char *cmd, char **env)
{
	char	**allpath;
	char	*path_env;
	char	*full_path;
	char	*path_part;
	int		i;

	i = 0;
	path_env = my_getenv("PATH", env);
	if (!path_env)
		return (cmd);
	allpath = ft_split(path_env, ':');
	if (!allpath)
		return (cmd);
	while (allpath[i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		full_path = join_and_check_access(path_part, cmd, allpath);
		if (full_path)
			return (full_path);
		i++;
	}
	ft_free_tab(allpath);
	return (cmd);
}

int main(int argc, char **argv, char **envp)
{
    int pipe_fd[2];
    pid_t pid;

    if (argc != 5)
        error_and_exit(1);
    if (pipe(pipe_fd) == -1)
        error_and_exit(2);
    pid = fork();
    if (pid == -1)
        error_and_exit(2);
    if (pid == 0)
        execute_child(argv, pipe_fd, envp);
    execute_parent(argv, pipe_fd, envp);
}

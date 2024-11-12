/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcentell <mcentell@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:07:24 by mcentell          #+#    #+#             */
/*   Updated: 2024/11/12 16:37:28 by mcentell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void error_and_exit(int exit_code)
{
    if (exit_code == 1)
    {
        fprintf(stderr, "Usage: ./pipex infile cmd1 cmd2 outfile\n");
    }
    else if (exit_code == 2)
    {
        perror("Error");
    }
    exit(exit_code);
}

char *my_getenv(char *name, char **env)
{
    int i;
    int j;
    char *sub;

    i = 0;
    while (env[i])
    {
        j = 0;
        while (env[i][j] && env[i][j] != '=')
            j++;
        sub = ft_substr(env[i], 0, j);
        if (!sub)
            return (NULL);
        if (ft_strcmp(sub, name) == 0)
        {
            free(sub);
            return (env[i] + j + 1);
        }
        free(sub);
        i++;
    }
    return (NULL);
}

int open_file(char *file, int flags)
{
    int ret;

    ret = open(file, flags, 0777);
    if (ret == -1)
    {
        perror("Error al abrir archivo");
        exit(1);
    }
    return (ret);
}

void ft_free_tab(char **tab)
{
    size_t i;

    i = 0;
    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}

char *join_and_check_access(char *path_part, char *cmd, char **allpath)
{
    char *full_path;

    full_path = ft_strjoin(path_part, cmd);
    free(path_part);
    if (access(full_path, F_OK | X_OK) == 0)
    {
        ft_free_tab(allpath);
        return (full_path);
    }
    free(full_path);
    return (NULL);
}

void exec(char *cmd, char **env)
{
    char **s_cmd;
    char *path;

    s_cmd = ft_split(cmd, ' ');
    path = get_path(s_cmd[0], env);
    if (execve(path, s_cmd, env) == -1)
    {
        ft_putstr_fd("pipex: command not found: ", 2);
        ft_putendl_fd(s_cmd[0], 2);
        ft_free_tab(s_cmd);
        exit(0);
    }
}


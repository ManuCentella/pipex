/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcentell <mcentell@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:26:56 by mcentell          #+#    #+#             */
/*   Updated: 2024/11/11 18:46:38 by mcentell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/inc/get_next_line.h" 
# include "../libft/inc/libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include "../src/pipex.h"

void exit_handler(int n_exit);
int open_file(char *file, int n);
char *my_getenv(char *name, char **env);
char *get_path(char *cmd, char **env);
void exec(char *cmd, char **env);
void ft_free_tab(char **tab);
void here_doc_put_in(char **av, int *p_fd);
void here_doc(char **av);
void do_pipe(char *cmd, char **env);

#endif
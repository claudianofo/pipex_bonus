/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 17:45:15 by cnorton-          #+#    #+#             */
/*   Updated: 2024/08/27 20:54:22 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include <stdlib.h>
# include "libft/libft.h"
# include <errno.h>
# include <string.h>
//# include <fcntl.h>

//get_next_line
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

# define EMPTY_STR      -1
# define NO_NL          -2

typedef struct   s_data
{
    int     ac;
    char    **av;
    char    **envp;
    int     *pipes;
    int     *pids;
    int     nb_cmds;
    int     here_doc;
    int     infile;
    int     outfile;
}               t_data;

char	*get_next_line(int fd);
t_data	init_data(int ac, char **av, char **envp);
void	free_array(char **array);
char	**ft_rm_quotes(char **cmd);
char	**split_pipex(char const *s);
void	ft_error(char *str1, char *str2, t_data *data);
void    clear_resources(t_data *data);
void    close_pipes(t_data *data);
void	check_args(int ac, char **av);
void	dup_in_out(int input, int output, t_data *data);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 20:54:08 by claudia           #+#    #+#             */
/*   Updated: 2024/08/25 21:10:35 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//Closes all pipe file descriptors stored in data
void    close_pipes(t_data *data)
{
    int i;

    i = 0;
    while (i < (data->nb_cmds - 1) * 2)
    {
        if (data->pipes[i])
            close(data->pipes[i]);
        i++;
    }
}

//Clears any resources in use in data struct:
//de-allocates memory, closes infile, outfile and pipes, deletes .here_doc
void    clear_resources(t_data *data)
{
    if (data)
    {
        if (data->infile != -1)
            close(data->infile);
        if (data->outfile != -1)
            close(data->outfile);
        if (data->pipes)
        {
            close_pipes(data);
            free(data->pipes);
        }
        if (data->here_doc == 1)
		    unlink(".here_doc");
        if (data->pids)
            free(data->pids);
    }
}

//Clears any resources currently in use, prints error message and exits program
void	ft_error(char *str1, char *str2, t_data *data)
{	
	clear_resources(data);
	ft_putstr_fd("Error: ", 2);
    printf("hello\n");
	ft_putstr_fd(str1, 2);
	ft_putendl_fd(str2, 2);
	exit (1);
}
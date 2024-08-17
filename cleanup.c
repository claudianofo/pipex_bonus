/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 20:54:08 by claudia           #+#    #+#             */
/*   Updated: 2024/08/17 22:54:15 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void close_pipes(t_data *data)
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
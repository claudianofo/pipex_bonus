/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 19:45:10 by claudia           #+#    #+#             */
/*   Updated: 2024/08/17 21:30:28 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	here_doc(t_data *data)
{
	char	*line;

	data->infile = open(".here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (data->infile == -1)
			error_msg("Unable to create here_doc temp file:", strerror(errno));
		while (1)
		{ 
			ft_putstr_fd(">", STDOUT_FILENO);
			line = get_next_line(STDIN_FILENO);
			if (line == NULL)
				break;
			if (ft_strlen(data->av[2]) + 1 == ft_strlen(line) 
				&& !ft_strncmp(line, data->av[2], ft_strlen(data->av[2] + 1)))
				break;
			ft_putstr_fd(line, data->infile);
			free(line);
		}
}

void	set_infile(t_data *data)
{
	if (data->here_doc == 1)
		here_doc(data);
	else
	{
		data->infile = open(data->av[1], O_RDONLY);
		if (data->infile == -1)
			error_msg("Unable to open infile:", strerror(errno));
	}
}

void	create_pipes(t_data *data)
{
	int	i;

	data->pipes = malloc(sizeof(int) * 2 * (data->nb_cmds - 1));
	if (!data->pipes)
	{
		clear_resources(data);
		error_msg("Malloc error while allocating pipes:", strerror(errno));
	}
	i = 0;
	while (i < data->nb_cmds)
	{
		if (pipe(&data->pipes[i * 2]) == -1)
		{
			clear_resources(data);
			error_msg("Unable to create pipes:", strerror(errno));
		}
		i++;
	}
}

t_data	init_data(int ac, char **av, char **envp)
{
	t_data	data;

	data.ac = ac;
	data.av = av;
	data.envp = envp;
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
		data.here_doc = 1;
	else
		data.here_doc = 0;                                  
	set_infile(&data);
	data.nb_cmds = ac - 3 - data.here_doc;
	create_pipes(&data);
	return (data);
}
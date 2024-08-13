/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 19:45:10 by claudia           #+#    #+#             */
/*   Updated: 2024/08/13 19:46:38 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	set_infile(t_data *data)
{
	char	*line;
	int		len;

	if (data->here_doc == 1)
	{
		data->infile = open(".here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (data->infile == -1)
			error_msg("Here_doc:", strerror(errno)); //TODO cleanup??
		//use GNL to copy from STDIN to .here_doc until limiter
		len = ft_strlen(data->av[2]);
		while (1)
		{ 
			ft_putstr_fd(">", STDOUT_FILENO);
			line = get_next_line(STDIN_FILENO);
			if (line == NULL)
				break; //TODO check this
			else if (ft_strncmp(line, data->av[2], len + 1))
				break; //TODO check this
			ft_putstr_fd(line, data->infile);
			free(line);
		}
	}
	else
	{
		data->infile = open(data->av[1], O_RDONLY);
		if (data->infile == -1)
			error_msg("Unable to open infile:", strerror(errno));//TODO cleanup??
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
	return (data);
}
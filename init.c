/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 19:45:10 by claudia           #+#    #+#             */
/*   Updated: 2024/08/27 21:12:37 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//Creates temporary .here_doc file and takes user input from STDIN using 
//get_next_line(). Checks each line of input for LIMITER set by user.
static void	here_doc(t_data *data)
{
	char	*line;

	data->infile = open(".here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (data->infile == -1)
		ft_error("Unable to create here_doc temp file:", strerror(errno), data);
	while (1)
	{ 
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break;
		if (ft_strlen(data->av[2]) + 1 == ft_strlen(line) 
			&& !ft_strncmp(line, data->av[2], ft_strlen(data->av[2] + 1)))
			break;
		ft_putstr_fd(line, data->infile);
		free(line);
	}
	close(data->infile);
}

//Infile is opened in read only mode
//if here_doc flag is true, calls here_doc() to write user input to
//temp .here_doc file
static void	set_infile(t_data *data)
{
	if (data->here_doc == 1)
	{	
		here_doc(data);
		data->infile = open(".here_doc", O_RDONLY);
	}
	else
		data->infile = open(data->av[1], O_RDONLY);
	if (data->infile == -1)
			ft_error("Unable to open infile:", strerror(errno), data);
}

//Opens outfile file in truncated mode (overwrites file contents if it already exists)
//Or in append mode if here_doc flag is true
static void	set_outfile(t_data *d)
{
	printf("out = %s\n", d->av[d->ac - 1]);
	if (d->here_doc == 1)
		d->outfile = open(d->av[d->ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		d->outfile = open(d->av[d->ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (d->outfile == -1)
		ft_error("Unable to open outfile", strerror(errno), d);
}	

//Allocates memory for pipes and creates them with pipe()
static void	create_pipes(t_data *data)
{
	int	i;

	data->pipes = malloc(sizeof(int) * 2 * (data->nb_cmds - 1));
	if (!data->pipes)
		ft_error("Malloc error while allocating pipes:", strerror(errno), NULL);
	i = 0;
	while (i < data->nb_cmds)
	{
		if (pipe(&data->pipes[i * 2]) == -1)
			ft_error("Unable to create pipes:", strerror(errno), data);
		i++;
	}
	i = 0;
	while (i < (data->nb_cmds - 1) * 2)
	{
		printf("pipe[%d] = %d\n", i, data->pipes[i]);
		i++;
	}
}

/*
Creates and returns a data struct
Allocates memory for pipes and pids. Pipes are created.
Opens infile and outfile
Checks for here_doc and sets here_doc flag if true,
and creates temporary file for here_doc and takes user input
*/
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
	set_outfile(&data);
	data.nb_cmds = ac - 3 - data.here_doc;
	create_pipes(&data);
	data.pids = malloc(sizeof(int) * (data.nb_cmds));
	if (!data.pids)
		ft_error("Malloc error while allocating pids", strerror(errno), &data);
	return (data);
}

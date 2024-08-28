/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:17:53 by cnorton-          #+#    #+#             */
/*   Updated: 2024/08/27 21:13:48 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//duplicates specified input and output to STDIN snd STDOUT FILENOs
//called by each child process
void	dup_in_out(int input, int output, t_data *data)
{
	if (output && dup2(input, STDIN_FILENO) == -1)
		ft_error("dup2", strerror(errno), data);
	write(1, "hellodup1\n", 10);
	if (dup2(output, STDOUT_FILENO) == -1 && write(1, "hellodupp\n", 10) == 10)
		ft_error("dup2", strerror(errno), data);
	write(1, "hellodup2\n", 10);
}

//checks if enough args were passed
//returns 1 if not enough args
void	check_args(int ac, char **av)
{
	if (ac < 5 || (ac < 6 && ft_strncmp(av[1], "here_doc", 9) == 0))
	{
		if (ac >= 2 && ft_strncmp(av[1], "here_doc", 9) == 0)
			ft_error("Not enough args, follow format:\n", 
				"./pipex here_doc LIMITER cmd1 cmd2 ... cmdn outfile", NULL);
		else
			ft_error("Not enough args. Follow format:\n", 
				"./pipex infile cmd1 cmd2 ... cmdn outfile", NULL);
	}
}

char	**ft_rm_quotes(char **cmd)
{
	int		i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i][0] == '\'')
			cmd[i] = ft_strtrim(cmd[i], "\'");
		if (cmd[i][0] == '\"')
			cmd[i] = ft_strtrim(cmd[i], "\"");
		i++;
	}
	return (cmd);
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

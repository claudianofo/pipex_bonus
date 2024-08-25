/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:17:53 by cnorton-          #+#    #+#             */
/*   Updated: 2024/08/25 19:00:21 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

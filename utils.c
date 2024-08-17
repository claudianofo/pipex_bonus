/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:17:53 by cnorton-          #+#    #+#             */
/*   Updated: 2024/08/17 21:55:23 by claudia          ###   ########.fr       */
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

void	ft_error(char *str1, char *str2, t_data *data)
{	
	clear_resources(data);
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(str1, 2);
	ft_putendl_fd(str2, 2);
	exit (1);
}

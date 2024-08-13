/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 21:08:35 by cnorton-          #+#    #+#             */
/*   Updated: 2024/08/08 17:25:22 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
* Outputs the string 's' to the file descriptor 'fd' followed by a newline.
*/
void	ft_putendl_fd(char *s, int fd)
{
	int	i;
	int	len;

	len = ft_strlen(s);
	i = 0;
	while (i < len)
	{
		write(fd, &s[i++], 1);
	}
	write(fd, "\n", 1);
}

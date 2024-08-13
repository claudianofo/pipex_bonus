/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 20:45:56 by cnorton-          #+#    #+#             */
/*   Updated: 2024/08/08 20:04:16 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	l;
	char	*new;

	l = ft_strlen(s1) + ft_strlen(s2);
	new = (char *)malloc(l + 1);
	if (!new)
		return (NULL);
	ft_strlcpy(new, s1, l + 1);
	ft_strlcat(new, s2, l + 1);
	return (new);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:25:46 by cnorton-          #+#    #+#             */
/*   Updated: 2024/08/25 20:54:12 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//This gnl_strjoin is a variation of the libft ft_strjoin.
//If no s1, s2 is copied in ft_strdup. If s1 does exist,
//it is freed after use.
char	*gnl_strjoin(char *s1, char const *s2)
{
	size_t	l;
	char	*new;

	if (!s1)
	{
		if (!s2)
			return (NULL);
		return (ft_strdup(s2));
	}	
	l = ft_strlen(s1) + ft_strlen(s2);
	new = (char *)malloc(l + 1);
	if (!new)
		return (NULL);
	ft_strlcpy(new, s1, l + 1);
	ft_strlcat(new, s2, l + 1);
	free(s1);
	return (new);
}

//header file defines EMPTY_STR as -1
//NO_NL as -2
//if string contains a new line, this function
//returns the index of the first new line
static int	check_line_end(char *s)
{
	size_t	i;
	size_t	len;

	if (!s || !s[0])
		return (EMPTY_STR);
	len = ft_strlen(s);
	i = 0;
	while (i < len)
	{
		if (s[i] == '\n' || s[i] == '\0')
			return (i);
		i++;
	}
	return (NO_NL);
}

//reads from fd to buff
//static variable (hold_str) becomes a duplicate
//of buff if empty, else it is concatonated with buff
static char	*read_to_buff(int fd, char *hold_str)
{
	int			readbyte;
	char		*buffer;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	while (check_line_end(hold_str) < 0)
	{
		readbyte = read(fd, buffer, BUFFER_SIZE);
		if (readbyte < 1 && hold_str == NULL)
		{
			free(buffer);
			return (NULL);
		}
		buffer[readbyte] = '\0';
		if (readbyte < BUFFER_SIZE)
		{
			hold_str = gnl_strjoin(hold_str, buffer);
			break ;
		}
		hold_str = gnl_strjoin(hold_str, buffer);
	}
	free(buffer);
	return (hold_str);
}

static void	*free_hold_str(char **hold_str)
{
	free(*hold_str);
	*hold_str = NULL;
	return (NULL);
}

/*
Returns one line of fd at a time as a string, or NULL if there is nothing
left to read or an error occured. Will return subsequent lines on subsequent calls.
Function can be compiled with "-D BUFFER_SIZE=n" flag to modify size of buffer
for read(). If flag isn't used, default is set as 5.
Note: buffer size can change the efficiency of the funciton, depending on
the relative size of the line. Small lines are processed more efficiently with small
buffer size and larger buffer size is more efficient for large lines.
*/
char	*get_next_line(int fd)
{
	static char	*hold_str = NULL;
	char		*tmp_str;
	char		*out;
	int			i;

	if (fd < 0 || BUFFER_SIZE < 1 || read(fd, 0, 0) == -1)
		return (free_hold_str(&hold_str));
	hold_str = read_to_buff(fd, hold_str);
	if (!hold_str)
		return (NULL);
	i = check_line_end(hold_str);
	if (i < 0)
	{
		out = ft_strdup(hold_str);
		free(hold_str);
		hold_str = NULL;
		return (out);
	}
	out = malloc(i + 2);
	ft_strlcpy(out, hold_str, i + 2);
	tmp_str = ft_strdup(&hold_str[i + 1]);
	free(hold_str);
	hold_str = tmp_str;
	return (out);
}

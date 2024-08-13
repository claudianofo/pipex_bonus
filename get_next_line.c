/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:25:46 by cnorton-          #+#    #+#             */
/*   Updated: 2024/08/12 21:22:01 by claudia          ###   ########.fr       */
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
//of buff if emtpy, else it is concatonated with buff
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

//hold_str is passed in as a double pointer
//so that the memory at hold_str can be successfully
//freed and set to null. The double pointer avoids creating
//a dangling pointer, which is when a pointer is pointing at
//unallocated memory. This happens because when you pass a single 
//pointer into a function, it will exist at a separate memory address 
//to the original pointer from the original function, however they 
//store the same value. You can free
//the memory from the pointer copy, but you can't set the original
//pointer to null. To get around this, the double pointer allows you
//to access the original memory location by passing in a pointer
//to the original hold_str pointer. 
static void	*free_hold_str(char **hold_str)
{
	free(*hold_str);
	*hold_str = NULL;
	return (NULL);
}

//calls read_to_buff then check_line_end, which returns
//-1 for empty str, -2 for no nl, or the index of the first nl.
//if there is no nl(indicating EOF) in hold_str, GNL duplicates 
//hold_str (out), frees hold str then returns out. 
//It creates a duplicate so that the static variable can be 
//cleared so it doesn't interfere if GNL is called again.
//If there is a nl, out becomes a copy of hold_str up until nl
//then hold_str becomes a copy of everything it contained
//after the nl.
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
/*
int	main(void)
{
	int	fd;
	setbuf(stdout, NULL);
	fd = open("1char.txt", O_RDONLY);
	printf("GNL1:%s", get_next_line(fd));
	printf("GNL2:%s", get_next_line(fd));
	printf("GNL3:%s", get_next_line(fd));
	printf("GNL4:%s", get_next_line(fd));
	printf("GNL5:%s", get_next_line(fd));
	printf("GNL6:%s", get_next_line(fd));
	printf("GNL7:%s", get_next_line(fd));
	printf("GNL8:%s", get_next_line(fd));
	close(fd);
	return (0);
}*/

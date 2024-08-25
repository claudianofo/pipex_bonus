/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 14:35:51 by cnorton-          #+#    #+#             */
/*   Updated: 2024/08/25 21:52:25 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//checks for PATH variable in envp using ft_strnstr
//splits PATH by directory (using ft_split and splitting by :)
//adds a slash and the cmd (potential_path/cmd) to the end of each directory
//checks through potential paths until finding an executable file
char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	int		i;
	char	*potential_path;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		potential_path = ft_strjoin_three(paths[i], "/", cmd);
		if (access(potential_path, F_OK | X_OK) == 0)
		{
			free_array(paths);
			return (potential_path);
		}
		free(potential_path);
		i++;
	}
	return (free_array(paths), NULL);
}

//calls split_pipex: an edited version of ft_split
//which splits cmd and its params by spaces unless inside quotation marks
//calls find_path which finds the path to an executable file 
//for the bash cmd inside the environmental pointer
//replaces current process with the bash command using execve
void	ft_exec(char *arg, t_data *data)
{
	char	**cmd;
	int		i;
	char	*path;
	char	*tmp_cmd;

	//printf("arg = %s\n", arg);
	cmd = split_pipex(arg);
	//printf("cmd[0] = %s\n", cmd[0]);//WHAT IS GOING ON WITH THE OUTPUTS????
	path = find_path(cmd[0], data->envp);
	i = 0;
	if (!path)
	{
		tmp_cmd = cmd[0];
		while (cmd[i])
			free(cmd[i++]);
		free(cmd);
		ft_error(tmp_cmd, ": command not found", data);
	}
	if (execve(path, cmd, data->envp) == -1)
		ft_error("Execve: ", strerror(errno), data);
}

/*
sets input and output depending on if its the first, last or middle child
if middle or last child, waits for the child processes to finish before
executing command
*/
void	child_process(t_data *d, int child_nb)
{
	if (child_nb == 0)
		dup_in_out(d->infile, d->pipes[1], d);
	else if (child_nb == d->nb_cmds - 1)
		dup_in_out(d->pipes[2 * child_nb - 2], d->outfile, d);
	else
		dup_in_out(d->pipes[2 * child_nb - 2], d->pipes[2 * child_nb + 1], d);
	close(d->infile);
	close(d->outfile);
	close_pipes(d);
	//if (child_nb > 0)
	//	waitpid(d->pids[child_nb - 1], NULL, 0); PROBELM HERE
	ft_exec(d->av[child_nb + 2 + d->here_doc], d);
}

//waits for last child process to finish, clears resources
//returns exit status of last child
int	parent_process(t_data *data, int last_child)
{
	int	wstatus;

	waitpid(data->pids[last_child], &wstatus, 0); //MAYBE PROBLEM HERE
	clear_resources(data);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	return (1);
}

//Checks for correct argument format. Accepts multiple pipes and here_doc with limiter.
//Initialises data (pipes, infile or heredoc, outfile and allocates for pids)
//Forks into multiple processes to execute commands; one child per command
//Clears any resources in parent process and returns exit status of last child
int	main(int ac, char **av, char **envp)
{
	t_data	data;
	int		child_nb;

	check_args(ac, av);
	data = init_data(ac, av, envp);
	child_nb = 0;
	while (child_nb < data.nb_cmds)
	{
		data.pids[child_nb] = fork();
		if (data.pids[child_nb] == -1)
			ft_error("Fork: ", strerror(errno), &data);
		if (data.pids[child_nb] == 0)
			child_process(&data, child_nb);
		child_nb++;
	}
	return (parent_process(&data, child_nb - 1));
}     

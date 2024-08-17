/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 14:35:51 by cnorton-          #+#    #+#             */
/*   Updated: 2024/08/17 23:21:23 by claudia          ###   ########.fr       */
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

	cmd = split_pipex(arg);
	path = find_path(cmd[0], data->envp);
	i = -1;
	if (!path)
	{
		while (cmd[++i])
			free(cmd[i]);
		free(cmd);
		ft_error(cmd[0], ": command not found", data);
	}
	if (execve(path, cmd, data->envp) == -1)
		ft_error("Execve: ", strerror(errno), data);
}

void dup_in_out(int input, int output, t_data *data)
{
	if (dup2(input, STDIN_FILENO) == -1)
		ft_error("dup2", strerror(errno), data);
	if (dup2(output, STDOUT_FILENO) == -1)
		ft_error("dup2", strerror(errno), data);
}

//opens infile
//infile is duplicated to STDIN
//write end of pipe is duplicated to STDOUT
//calls ft_exec function for cmd1
void	child_process(t_data *data, int child_nb)
{
	if (child_nb == 0)
		dup_in_out(data->infile, data->pipes[1], data);
	else if (child_nb == data->nb_cmds - 1)
		
	dup2(data->infile, STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(data->infile);
	close(pipe_fd[1]);
	ft_exec(argv[2], envp);
}

//opens or creates outfile if it doesn't exits
//0644 is the default permissions of files created by bash
//read end of pipe is duplicated as STDIN
//outfile is duplicated as STDOUT
//calls ft_exec function for cmd2
void	parent_process(t_data *data)
{
	int	outfile;

	close(pipe_fd[1]);
	outfile = open(argv[4], O_RDWR | O_CREAT | O_TRUNC, 0644); //TODO: add condition for here_doc
	if (outfile == -1)
	{
		perror("ERROR opening outfile");
		exit(EXIT_FAILURE);
	}
	dup2(pipe_fd[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	close(pipe_fd[0]);
	ft_exec(argv[3], envp);
}

//checks if enough args were passed
//returns 1 if not enough args
void	check_args(int ac, char **av)
{
	if (ac < 5 || (ac < 6 && ft_strncmp(av[1], "here_doc", 9) == 0))
	{
		if (ac >= 2 && ft_strncmp(av[1], "here_doc", 9) == 0)
			ft_error("Not enough args, follow format:\n", 
				"./pipex here_doc LIMITER cmd1 cmd2 ... cmdn file2", NULL);
		else
			ft_error("Not enough args. Follow format:\n", 
				"./pipex file1 cmd1 cmd2 ... cmdn file2", NULL);
	}
}

//checks for correct no. of arguments
//forks into 2 processes
//waitpid is called to ensure that parent process 
//doesn't terminate before child process finishes
int	main(int ac, char **av, char **envp)
{
	t_data	*data;
	int		pipe_fd[2];
	pid_t	pid;
	int		child_nb;

	check_args(ac, av);
	data = init_data(ac, av, envp);
	pid = fork();
	child_nb = 0;
	while (child_nb < data->ac)
	{
		data->pids[child_nb] = fork();
		if (data->pids[child_nb] == -1)
			ft_error("Fork: ", strerror(errno), data);
		if (data->pids[child_nb] == 0)
			child(data, child_nb);
		child_nb++;
	}
	if (data->here_doc == 1)
		    unlink(".here_doc");
	parent(data);
	return (0);
}     

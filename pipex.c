/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 14:35:51 by cnorton-          #+#    #+#             */
/*   Updated: 2024/08/17 21:29:27 by claudia          ###   ########.fr       */
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
void	ft_exec(char *arg, char **envp)
{
	char	**cmd;
	int		i;
	char	*path;

	cmd = split_pipex(arg);
	path = find_path(cmd[0], envp);
	i = -1;
	if (!path)
	{
		while (cmd[++i])
			free(cmd[i]);
		free(cmd);
		perror("Error ??");
		exit(EXIT_FAILURE);
	}
	if (execve(path, cmd, envp) == -1)
		perror("error execve");
	exit(EXIT_FAILURE);
}

//opens infile
//infile is duplicated to STDIN
//write end of pipe is duplicated to STDOUT
//calls ft_exec function for cmd1
void	child_process(t_data *data, char **argv, char **envp, int *pipe_fd)
{
	close(pipe_fd[0]);
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
void	parent_process(char **argv, char **envp, int *pipe_fd)
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
			error_msg("Not enough args, follow format:\n", 
				"./pipex here_doc LIMITER cmd1 cmd2 ... cmdn file2");
		else
			error_msg("Not enough args. Follow format:\n", 
				"./pipex file1 cmd1 cmd2 ... cmdn file2");
	}
}

//checks for correct no. of arguments
//forks into 2 processes
//waitpid is called to ensure that parent process 
//doesn't terminate before child process finishes
int	main(int ac, char **av, char **envp)
{
	t_data	data;
	int		pipe_fd[2];
	pid_t	pid;

	check_args(ac, av);
	data = init_data(ac, av, envp);
	pid = fork();
	if (pid == -1)
		perror("ERROR forking");
	else if (pid == 0)
		child_process(&data, av, envp, pipe_fd);
	else
	{
		parent_process(av, envp, pipe_fd);
		waitpid(pid, NULL, 0);
	}
	return (0);
}     

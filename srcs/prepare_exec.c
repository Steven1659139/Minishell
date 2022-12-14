/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 10:13:20 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/12/05 13:44:25 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Avec l'ancienne fonction, ca ne fait que verifier que 
//le 1ere commande est un builtin.  
void	is_builtin(t_info *info)
{
	char	*token;
	int		i;

	if (!info->command_lines)
		return ;
	i = 0;
	while ((i <= info->nb_of_pipe) && info->command_lines[i].list_token)
	{
		token = info->command_lines[i].list_token->token;
		if (!ft_strncmp(token, "pwd", 3) && ft_strlen(token) == 3)
			info->command_lines[i].builtin = 1;
		else if (!ft_strncmp(token, "env", 3) && ft_strlen(token) == 3)
			info->command_lines[i].builtin = 1;
		else if (!ft_strncmp(token, "cd", 2) && ft_strlen(token) == 2)
			info->command_lines[i].builtin = 1;
		else if (!ft_strncmp(token, "exit", 4) && ft_strlen(token) == 4)
			info->command_lines[i].builtin = 1;
		else if (!ft_strncmp(token, "export", 6) && ft_strlen(token) == 6)
			info->command_lines[i].builtin = 1;
		else if (!ft_strncmp(token, "echo", 4) && ft_strlen(token) == 4)
			info->command_lines[i].builtin = 1;
		else if (!ft_strncmp(token, "unset", 5) && ft_strlen(token) == 5)
			info->command_lines[i].builtin = 1;
		i++;
	}
}

void	find_execve_path(t_info *info, t_command_line *cmd_line)
{
	char	**path;
	int		i;
	int		j;

	path = info->paths;
	if (!path)
		return ;
	i = 0;
	while (i <= info->nb_of_pipe)
	{
		j = 0;
		while (cmd_line[i].list_token && cmd_line[i].builtin != 1 && path[j])
		{
			find_path_of_command(&cmd_line[i], path[j]);
			if (cmd_line[i].path)
				break ;
			j++;
		}
		i++;
	}
}

void	find_path_of_command(t_command_line *cmd_line, char *path)
{
	char	*temp_exe;
	char	*temp_path;

	if (access((*cmd_line).list_token->token, X_OK | F_OK) != -1)
		(*cmd_line).path = ft_strdup((*cmd_line).list_token->token);
	else
	{
		temp_path = ft_strjoin(path, "/");
		temp_exe = ft_strjoin(temp_path, (*cmd_line).list_token->token);
		free(temp_path);
		if (access(temp_exe, X_OK | F_OK) != -1)
			(*cmd_line).path = temp_exe;
		else
			free(temp_exe);
	}
}

void	create_execve_argv(t_info	*info, t_command_line *cmd_line)
{
	t_token	*list;
	int		i;
	int		j;
	int		len;

	i = 0;
	if (!cmd_line)
		return ;
	while (i <= info->nb_of_pipe)
	{
		j = 0;
		list = cmd_line[i].list_token;
		if (list)
		{
			len = ft_lstsize_token(list);
			cmd_line[i].argv = malloc((len + 1) * sizeof(char *));
			while (list)
			{
				cmd_line[i].argv[j++] = ft_strdup(list->token);
				list = list->next;
			}
			cmd_line[i].argv[j] = 0;
		}
		i++;
	}
}

void	prepare_data_for_execution(t_info *info)
{
	is_builtin(info);
	find_execve_path(info, info->command_lines);
	create_execve_argv(info, info->command_lines);
}

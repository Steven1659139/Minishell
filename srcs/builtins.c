/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 09:45:30 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/15 23:06:55 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	pwd(t_info *info)
{
	if (getcwd(info->pwd, 4096))
		printf("%s\n", getcwd(info->pwd, 4096));
	else
		printf("%s\n", info->pwd);
}

// int	is_all_n(char *token)
// {
// 	while (*token)
// 	{
// 		token++;
// 		printf("*token = %c\n", *token);
// 		if (*token == '\0')
// 			return (1);
// 		else if (*token != 'n')
// 			return (0);
// 	}
// 	return (1);
// }

void	echo(t_info *info)
{
	t_token	*token_list;
	int		i;
	int		n;

	n = 0;
	i = 0;
	token_list = info->command_lines[info->index].list_token->next;
	quote_remover(info);
	if (token_list)
	{
		while (is_n(token_list))
		{
			token_list = token_list->next;
			n++;
		}
	}
	else
	{
		printf("\n");
		return ;
	}
	while (token_list)
	{
		if (token_list->prev && token_list->prev->space_flag == 1 && i > 0)
			printf(" %s", token_list->token);
		else
			printf("%s", token_list->token);
		i++;
		token_list = token_list->next;
	}
	if (!n)
		printf("\n");
}

void	cd(t_info *info)
{
	char	*new_path;
	char	pwd[4096];
	char	oldpwd[4096];
	char	*line;

	getcwd(oldpwd, 4096);
	if (info->command_lines[info->index].args)
		new_path = info->command_lines[info->index].args;
	else
		new_path = getenv("HOME");
	if (chdir(new_path) != 0 && ((ft_strncmp(new_path, ".", 1) && \
	ft_strncmp(new_path, "..", 2)) || !ft_strncmp(new_path, "...", 3)))
		printf("cd: %s: No such file or directory\n", new_path);
	getcwd(pwd, 4096);
	line = search_line(info->envp, "PWD=");
	line = ft_strjoin("PWD=", pwd);
	ft_strlcpy(search_line(info->envp, "PWD="), line, ft_strlen(line) + 1);
	free(line);
	line = ft_strjoin("OLDPWD=", oldpwd);
	free(line);
	ft_strlcpy(search_line(info->envp, "OLDPWD="), line, ft_strlen(line) + 1);
}

void	export(t_info *info)
{
	int		i;
	int		j;
	char	*str;
	char	*line;

	i = 0;
	j = 0;
	if (info->command_lines[info->index].cmd_and_args[i + 1])
	{
		while (info->command_lines[info->index].cmd_and_args[i + 1])
		{
			if (check_arg_export(info->command_lines[info->index] \
			.cmd_and_args[i + 1]))
			{
				str = until_chr(info->command_lines[info->index] \
				.cmd_and_args[i + 1], '=');
				line = search_line(info->envp, str);
				if (line)
				{
					free(str);
					ft_strlcpy(line, info->command_lines[info->index] \
					.cmd_and_args[i + 1], ft_strlen(line) + 1);
				}
				else
					info->envp = tab_join(info->envp, info->command_lines[info->index] \
					.cmd_and_args[i + 1]);
			}
			i++;
		}
	}
	else
	{
		while (info->envp[i])
		{
			printf("declare -x ");
			while (info->envp[i][j] != '=' && info->envp[i][j] != '\0')
			{
				printf("%c", info->envp[i][j]);
				j++;
			}
			str = ft_strchr(info->envp[i], '=');
			printf("=\"%s\"\n", ++str);
			i++;
			j = 0;
		}
	}
}

void	unset(t_info *info)
{
	int		i;
	char	*str;

	i = 0;
	while (info->command_lines[info->index].cmd_and_args[i + 1])
	{
		if (check_arg_unset(info->command_lines[info->index].cmd_and_args[i + 1], info))
		{
			str = ft_strjoin(info->command_lines[info->index].cmd_and_args[i + 1], "=");
			info->envp = tab_trunc(info->envp, str, ft_strlen(str));
			free(str);
		}
		i++;
	}
}

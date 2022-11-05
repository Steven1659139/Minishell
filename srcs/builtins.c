/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 09:45:30 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/05 16:36:41 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	remove_quote(t_token *token_list)
{
	char	*temp;
	char	chr;

	chr = simple_or_double(token_list->token);
	if (chr != 32)
	{
		temp = token_list->token;
		token_list->token = ft_substr(token_list->token, 1, ft_strlen(token_list->token) - 2);
		free(temp);
		token_list->flag_quote = 1;
	}
	else
		token_list->flag_quote = 0;
}

void	pwd(t_info *info)
{
	char	*pwd;

	pwd = search_line(info->envp, "PWD=");
	pwd = ft_strchr(pwd, '=');
	printf("%s\n", ++pwd);
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

int	is_n(t_token *node)
{
	int	i;

	i = 2;

	if (node)
	{
		if (ft_strlen(node->token) == 2 && !ft_strncmp(node->token, "-n", 2))
			return (1);
		while (node->token[i])
		{
			if (!(node->token[i] == 'n'))
				return (0);
			i++;
		}
		return (1);
	}
	return (0);
}

void	echo(t_info *info)
{
	t_token	*token_list;
	int		i;
	int		n;

	// printf("IN ECHO\n");
	
	n = 0;
	i = 0;
	token_list = info->command_lines[info->index].list_token->next;
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
		{
			remove_quote(token_list);
			printf(" %s", token_list->token);
		}
		else
		{
			remove_quote(token_list);
			printf("%s", token_list->token);
		}
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
	if (chdir(new_path) != 0 && ((ft_strncmp(new_path, ".", 1) && ft_strncmp(new_path, "..", 2)) || !ft_strncmp(new_path, "...", 3)))
		printf("cd: %s: No such file or directory\n", new_path);
	getcwd(pwd, 4096);
	line = search_line(info->envp, "PWD=");
	line = ft_strjoin("PWD=", pwd);
	ft_strlcpy(search_line(info->envp, "PWD="), line, ft_strlen(line) + 1);
	line = ft_strjoin("OLDPWD=", oldpwd);
	ft_strlcpy(search_line(info->envp, "OLDPWD="), line, ft_strlen(line) + 1);
}

void	export(t_info *info)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	if (info->command_lines[info->index].args)
		info->envp = tab_join(info->envp, info->command_lines[info->index].args);
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

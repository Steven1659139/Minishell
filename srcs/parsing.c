/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:07:47 by slavoie           #+#    #+#             */
/*   Updated: 2022/10/27 14:26:25 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	split_token(char *token, t_info *info)
{
	int i;
	int x;

	i = 0;
	x = 0;
	info->last_position = token;
	info->command_lines = ft_calloc(info->nb_of_pipe + 1, sizeof(t_command_line));
	init_struct(info->command_lines, info);
	while (*info->last_position)
	{
		skip_space(info);
		if (*info->last_position == '|')
		{
			info->command_lines[i].list_token = info->list_token;
			// lst_print_token(&info->command_lines[i].list_token);
			
			// ft_lstlast_token(info->command_lines[i].list_token)->next = NULL;
			info->list_token = NULL;
			info->last_position++;
			i++;
			// printf("i = %d\n", i);
		}
		ft_lstaddback_token(&info->list_token, ft_lstnew_token(search_another_one(info->last_position, simple_or_double(info->last_position), info)));
		// info->token = tab_join(info->token, search_another_one(info->last_position, simple_or_double(info->last_position), info));
		skip_space(info);
	}
	// printf("i = %d\n", i);
	// i++;
	info->command_lines[i].list_token = info->list_token;
	info->list_token = NULL;
	// lst_print_token(&info->command_lines[i].list_token);

	// x = i;
	// i = 0;
	// while (x >= i)
	// {
	// 	printf("i = %d\n", i);
	// 	lst_print_token(&info->command_lines[i++].list_token);
	// }
	// // print_tab(info->token);
	// lst_print_token(&info->list_token);
}

char	*get_command(t_token *list_token)
{
	if (!list_token)
		return (NULL);
	return(list_token->token);
}

char	*get_args(t_token	*list_token)
{
	char *args;

	args = NULL;
	if (list_token)
		list_token = list_token->next;
	while (list_token)
	{
		args = ft_strjoin(args, list_token->token);
		args = ft_strjoin(args, " ");
		list_token = list_token->next;
	}
	return (args);
}

void	fill_command_lines(t_info *info)
{
	int	i;

	i = 0;

	while (i <= info->nb_of_pipe)
	{
		info->command_lines[i].command = get_command(info->command_lines[i].list_token);
		printf("command [%d] = %s\n", i, info->command_lines[i].command);
		info->command_lines[i].args = get_args(info->command_lines[i].list_token);
		printf("args [%d] = %s\n", i, info->command_lines[i].args);
		i++;
	}
	//info->command_lines[i].command = get_command(info->list_token);
	//printf("command [%d] = %s\n", i, info->command_lines[i].command);


	// info->command_lines[i].args = get_args(info->list_token);
	// printf("args [%d] = %s\n", i, info->command_lines[i].args);


	
}

/*
	Cherche la prochaine occurence de "c" et renvoie le token
*/
char	*search_another_one(char *str, char c, t_info *info)
{
	char	*token;
	char	*start;
	int		len;

	start = str;
	len = 0;
	str++;
	len++;
	while (*str != c)
	{
		if (*str == '\0' || (c == 32 && (*str == 34 || *str == 39)))
		{
			info->last_position = str;
			token = ft_substr(start, 0, len);
			return (token);
		}
		str++;
		len++;
	}
	if (c != 32)
	{
		info->last_position = ++str;
		len++;
	}
	else
		info->last_position = str;
	// printf("last_position search = %c\n", *info->last_position);

	token = ft_substr(start, 0, len);
	return (token);
}
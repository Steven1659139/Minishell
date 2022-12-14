/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 16:59:53 by slavoie           #+#    #+#             */
/*   Updated: 2022/12/07 13:21:09 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	routine_split_token(t_info *info)
{
	int		type_quote;
	char	*token;

	skip_space(info);
	token = check_chevron(info);
	if (!(parse_error(info)))
		return ;
	type_quote = simple_or_double(info->last_position);
	if (!token)
		token = search_another_one(info->last_position, type_quote, info);
	ft_lstaddback_token(&info->list_token, ft_lstnew_token(token));
	ft_lstlast_token(info->list_token)->flag_quote = type_quote;
	skip_space(info);
	trim_space(info, " \t\n\r\v|");
	info->nb_token++;
}

int	little_split_token(t_info *info, int i)
{
	while (*info->last_position)
	{
		routine_split_token(info);
		if (info->err_chevron || info->err_happen)
			break ;
		if (*info->last_position == '|')
		{
			if (!(*info->list_token->token))
			{
				ft_putstr_fd("Minishell: syntax \
				error near unexpected token `|'\n", 2);
				info->err_happen = 1;
				break ;
			}
			info->command_lines[i].list_token = info->list_token;
			info->list_token = NULL;
			info->last_position++;
			i++;
		}
	}
	return (i);
}

void	split_token(char *token, t_info *info)
{
	int	i;

	i = 0;
	info->last_position = token;
	info->command_lines = \
	ft_calloc(info->nb_of_pipe + 2, sizeof(t_command_line));
	init_command_lines(info->command_lines, info);
	i = little_split_token(info, i);
	info->command_lines[i].list_token = info->list_token;
	info->list_token = NULL;
}

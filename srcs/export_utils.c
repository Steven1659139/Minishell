/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 14:16:16 by slavoie           #+#    #+#             */
/*   Updated: 2022/12/07 13:34:40 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	export_routine(t_info *info, char *str, int i)
{	
	char	*line;

	str = until_chr(info->command_lines[info->index] \
	.argv[i + 1], '=');
	line = search_line(info->envp, str);
	if (line)
	{
		info->envp = tab_trunc(info->envp, str, ft_strlen(str));
		info->envp = tab_join(info->envp, \
		info->command_lines[info->index].argv[i + 1]);
	}
	else
	{
		info->envp = tab_join(info->envp, \
		info->command_lines[info->index].argv[i + 1]);
	}
	free(str);
}

void	export_no_args(t_info *info, char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
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
	info->exit_code = 0;
}

void	echo_routine(t_token *token_list)
{
	int		i;

	i = 0;
	while (token_list)
	{
		if (token_list->prev && token_list->prev->space_flag == 1 && i > 0)
			printf(" %s", token_list->token);
		else
			printf("%s", token_list->token);
		i++;
		token_list = token_list->next;
	}
}

void	little_main_routine(char *line, t_info *info)
{
	if (*line != '|')
	{
		info->nb_of_pipe = how_many(info, line, '|');
		split_token(line, info);
		free(line);
		if (info->command_lines->list_token && !info->err_happen)
		{
			if (search_for_redirection(info))
			{
				var_expansion(info->command_lines, info);
				put_token_toghther(info);
				quote_remover(info);
				fill_command_lines(info);
				prepare_data_for_execution(info);
				execution(info, info->command_lines);
			}
		}
	}
	else
	{
		ft_putstr_fd("Minishell: syntax error near unexpected token `|'\n", 2);
		info->exit_code = 258;
	}
}

void	routine(t_info *info, char *line)
{
	if (close_quote_checker(info, line))
		;
	else
	{
		ft_putstr_fd("Les quotes ne sont pas fermés.\n", 2);
		free(line);
		return ;
	}
	little_main_routine(line, info);
	free_struct_command_line(info);
}

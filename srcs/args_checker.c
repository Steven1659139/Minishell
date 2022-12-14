/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 17:42:30 by slavoie           #+#    #+#             */
/*   Updated: 2022/12/07 13:34:17 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_arg_export(char *arg, t_info *info)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[i]) && arg[i] != '_')
	{
		ft_putstr_fd("Minishell: export: '", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		info->exit_code = 1;
		return (0);
	}
	while (arg[i])
	{
		if (arg[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	check_arg_unset(char *arg, t_info *info)
{
	if (!ft_isalpha(*arg) && *arg != '_')
	{
		ft_putstr_fd("Minishell: unset: '", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		info->exit_code = 1;
		return (0);
	}
	return (1);
}

char	*input_chevron(t_info *info, int i)
{
	char	*token;

	token = NULL;
	if (info->last_position[i + 2] == '<')
		ft_putstr_fd("Minishell: syntax error near unexpected token '<'\n", 2);
	else if (info->last_position[i + 1] == '<')
	{
		token = ft_substr(info->last_position, i, 2);
		info->last_position++;
		info->last_position++;
		return (token);
	}
	else if (!(info->last_position[i + 1] == '<'))
	{
		token = ft_substr(info->last_position, i, 1);
		info->last_position++;
		return (token);
	}
	return (token);
}

char	*output_chevron(t_info *info, int i)
{
	char	*token;

	token = NULL;
	if (info->last_position[i + 2] == '>')
		ft_putstr_fd("Minishell: syntax error near unexpected token '>'\n", 2);
	else if (info->last_position[i + 1] == '>')
	{
		token = ft_substr(info->last_position, i, 2);
		info->last_position++;
		info->last_position++;
		return (token);
	}
	else if (!(info->last_position[i + 1] == '>'))
	{
		token = ft_substr(info->last_position, i, 1);
		info->last_position++;
		return (token);
	}
	return (NULL);
}

char	*check_chevron(t_info *info)
{
	int	i;

	i = 0;
	if (ft_strnstr(info->last_position, "<<<", 4))
		info->err_chevron = 1;
	else if (ft_strnstr(info->last_position, ">>>", 4))
		info->err_chevron = 2;
	else if (info->last_position[i] == '<' && search_next_chevron(info, i))
		info->err_chevron = 1;
	else if (info->last_position[i] == '>' && search_next_chevron(info, i))
		info->err_chevron = 1;
	else if (info->last_position[i] == '<' && search_next_pipe(info))
		return (input_chevron(info, i));
	else if (info->last_position[i] == '>' && search_next_pipe(info))
		return (output_chevron(info, i));
	return (NULL);
}

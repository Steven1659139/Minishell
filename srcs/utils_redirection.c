/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 22:10:20 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/12/02 17:13:19 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

void	delete_tokens(t_token **list)
{
	t_token	*temp;
	t_token	*prev;
	int		i;
	int		nodes_to_erase;

	i = 0;
	temp = NULL;
	prev = NULL;
	nodes_to_erase = 0;
	if ((*list)->next)
		nodes_to_erase = 1;
	while (i <= nodes_to_erase)
	{
		temp = *list;
		prev = (*list)->prev;
		*list = (*list)->next;
		ft_lstdelone_token(temp, free);
		temp = NULL;
		if (*list)
			(*list)->prev = prev;
		if (prev)
			prev->next = *list;
		i++;
	}
}

int	is_redirection(t_token *list)
{
	if ((ft_strncmp(list->token, "<", 2) == 0) || \
	(ft_strncmp(list->token, "<<", 2) == 0) || \
		(ft_strncmp(list->token, ">", 2) == 0) || \
		(ft_strncmp(list->token, ">>", 2) == 0))
		return (1);
	else
		return (0);
}

void	delete_redirection_tokens(t_token *list_token, t_token **list_addr)
{
	t_token	*list;

	list = list_token;
	while (list)
	{
		if (is_redirection(list) == 1 && list->prev)
			delete_tokens(&list);
		else if (is_redirection(list) == 1 && !list->prev)
		{
			delete_tokens(&list);
			*list_addr = list;
		}
		if (list && is_redirection(list) == 0)
			list = list->next;
	}
}

void	input_redirection(t_command_line *cmd_line, char *infile)
{
	if (!cmd_line->error_infile)
	{
		cmd_line->file_after_chevron = 1;
		if (cmd_line->fd_in != 0)
			close(cmd_line->fd_in);
		cmd_line->fd_in = open(infile, O_RDWR);
		if (cmd_line->fd_in == -1)
		{
			cmd_line->error_infile = ft_strdup(infile);
			cmd_line->fd_in = 0;
		}
	}
}

void	manage_heredoc_fds(t_info *info, t_command_line *cmd_line, int *fd)
{
	close (fd[1]);
	if (cmd_line->fd_in != 0)
	{
		close (cmd_line->fd_in);
		cmd_line->fd_in = 0;
	}
	if (info->heredoc == 1)
		close(fd[0]);
	else
		cmd_line->fd_in = fd[0];
}

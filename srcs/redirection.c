/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:55:32 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/31 15:02:50 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Qu'arrive-til si le fichier est deja cree? On ne veut pas remplacer le fd? 
void	append_document(t_command_line *chunk, char *outfile)
{
	if (chunk->fd_out != 1)
		close(chunk->fd_out);
	chunk->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (chunk->fd_in == -1)
	{
		printf("bash: %s: %s\n", outfile, strerror(errno));
	}
	printf("The fd associated with test2.txt est: %d\n", chunk->fd_out);
}

void	open_outfile(t_command_line *chunk, char *token)
{	
	if (chunk->fd_out != 1)
		close(chunk->fd_out);
	chunk->fd_out = open(token, O_TRUNC | O_CREAT | O_RDWR, 0644);
	printf("The fd associated with test2.txt est: %d\n", chunk->fd_out);
}

// Si j'ai plusieurs heredoc comment leur donner des noms differents? 
void	create_heredoc(t_command_line *chunk, char *delimiter)
{
	char	*line;
	
	if (chunk->fd_in != 0)
		close(chunk->fd_in);
	chunk->fd_in = open("heredoc.txt", O_TRUNC | O_CREAT | O_RDWR, 0644);
	while (1)
	{
		line = readline(">");
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break;
		}
		write(chunk->fd_in, line, ft_strlen(line));
		write(chunk->fd_in, "\n", 1);
		free(line);
	}
}

void	open_infile(t_command_line *cmd_line, t_token *list_token)
{
	cmd_line->fd_in = open(list_token->next->token, O_RDWR);
	if (cmd_line->fd_in == -1 && !(cmd_line->error_infile))
		cmd_line->error_infile = list_token->next->token;
}

int	is_redirection_operator(t_token	*list)
{
	if ((ft_strncmp(list->token, "<", 2) == 0) || ft_strncmp(list->token, "<<", 2) == 0 || 
		(ft_strncmp(list->token, ">", 2) == 0) || ft_strncmp(list->token, ">>", 2) == 0)
		return (1);
	else
		return (0);
}

void	delete_tokens(t_token **list)
{
	t_token	*temp;
	t_token	*prev;
	int 	i;
	int		nodes_to_erase;

	i = 0;
	nodes_to_erase = 0;
	if ((*list)->next)
		nodes_to_erase = 1;
	while (i <= nodes_to_erase)
	{
		temp = *list;
		prev = (*list)->prev;
		*list = (*list)->next;
		free (temp);
		temp = NULL;
		if (*list)
			(*list)->prev = prev;
		if (prev)
			prev->next = *list;
		i++;
	}
}

void	delete_redirection_tokens(t_token *list_token, t_token **list_addr)
{
	t_token *list;
	t_token	*tmp;
	
	list = list_token;
	tmp = NULL;
	while (list)
	{
		if (is_redirection_operator(list) == 1 && list->prev)
			delete_tokens(&list);
		else if (is_redirection_operator(list) == 1 && !list->prev)
		{
			delete_tokens(&list);
			*list_addr = list;
		}
		if (list)
			list = list->next; // Des que je viens ici cela change, list et list token pour la meme chose. 
	}
}

void	redirection(t_info	*info)
{
	t_command_line	*chunk;
	t_token			*list;
	int	i;

	i = 0;
	lst_print_token(&info->command_lines[i].list_token);
	while(i <= info->nb_of_pipe)
	{
		chunk = &info->command_lines[i];
		list = info->command_lines[i].list_token;
		while (list) 
		{
			if ((ft_strncmp(list->token, "<", 2) == 0) && list->next)
				open_infile(chunk, list);
			else if ((ft_strncmp(list->token, "<<", 2) == 0) && list->next)
				create_heredoc(chunk, list->next->token);
			else if ((ft_strncmp(list->token, ">", 2) == 0) && list->next)
				open_outfile(chunk, list->next->token);
			else if ((ft_strncmp(list->token, ">>", 2) == 0) && list->next)
				append_document(chunk, list->next->token);
			list = list->next;
		}
		delete_redirection_tokens(info->command_lines[i].list_token, &info->command_lines[i].list_token);
		i++;
	}
	lst_print_token(&info->command_lines[0].list_token);
}

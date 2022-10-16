/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 16:10:51 by slavoie           #+#    #+#             */
/*   Updated: 2022/10/14 14:58:55 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_lstsize_token(t_token *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		lst = lst->next;
		size++;
	}
	return (size);
}

void	ft_lstdelone_token(t_token *lst, void (*del)(void *))
{
	if (del)
		del(lst->token);
	free(lst);
}

void	ft_lstclear_token(t_token **lst, void (*del) (void *))
{
	t_token	*elem;
	int		i;

	i = ft_lstsize_token(*lst);
	while (i != 0)
	{
		elem = (*lst)->next;
		ft_lstdelone_token(*lst, del);
		*lst = elem;
		i--;
	}
}


void lst_print_token(t_token **list)
{
    t_token *node;
    int i;

    node = *list;
    i = 0;

    while (node)
    {
        printf("node[%d] = %s\n", i, (char *)node->token);
        node = node->next;
        i++;
    }
}

t_token	*ft_lstnew_token(char *content)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(*new));
	if (!new)
		return (0);
	new->token = content;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_token	*ft_lstlast_token(t_token *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstaddback_token(t_token **alst, t_token *new)
{
	t_token	*last;
	t_token	*first;

	first = *alst;
	if (!*alst)
	{
		*alst = new;
		return ;
	}
	last = ft_lstlast_token(*alst);
	new->prev = last;
	last->next = new;
	new->next = NULL;
}
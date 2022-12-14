/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 09:10:15 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/29 15:01:33 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
    sig_handler() function enumerated the different actions
    to be realized when SIGINT signal is sent to kernel. 
    1- Print newline on terminal
    2- rl_on_new_line tell terminal, that readline() function will be reading
    from this new line. This function normally following a \n
    3- rl_replace_line (char *text, int clear_undo): 
    if any text was entered prior to pressing Ctrl+C,
    rl_replace line takes the text entered (found in buffer) 
    and replace it by whatever is given as first argument.
    4- rl_redisplay changes what's display on screen to change for what
    is in rl_buffer.
*/
void	signal_inside_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		close(g_fd_in);
		exit(1);
	}
}

void	signal_outside_heredoc(int signum)
{
	if (signum == SIGINT)
		ft_putstr_fd("\n", 2);
}

void	enable_signals_minishell(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	signal_child(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	else if (signum == SIGQUIT)
		ft_putstr_fd("Quit: 3\n", 2);
}

void	signal_parent(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

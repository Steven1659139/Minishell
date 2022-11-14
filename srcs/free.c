/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:53:06 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/14 17:11:29 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_struct_command_line(t_info *info)
{
	int	i;

	i = 0;
	while (i <= info->nb_of_pipe)
	{
		table_flip(info->command_lines[i].cmd_and_args);
		free(info->command_lines[i].args);
		free(info->command_lines[i].merge_path_cmd);
		if (info->command_lines[i].fd_in != 0)
			close (info->command_lines[i].fd_in);
		if (info->command_lines[i].fd_out != 1)
			close (info->command_lines[i].fd_out);
		free(info->command_lines[i].error_infile);
		i++;
	}	
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 12:31:16 by amal              #+#    #+#             */
/*   Updated: 2025/06/09 12:31:39 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	restore_std(int saved_fd, int std_fd)
{
	if (dup2(saved_fd, std_fd) == -1)
		ft_error("dup2 restore");
	close(saved_fd);
}

void	setup_redir(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO && in_fd >= 0)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			ft_error("dup2 in_fd");
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO && out_fd >= 0)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			ft_error("dup2 out_fd");
		close(out_fd);
	}
}

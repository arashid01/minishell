/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:23:48 by nora              #+#    #+#             */
/*   Updated: 2025/06/07 18:26:28 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_sigint_heredoc(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	g_exit_status = 1;
	exit(1);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

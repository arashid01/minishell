/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 06:09:15 by amal              #+#    #+#             */
/*   Updated: 2025/06/06 03:06:56 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int g_signal_status = 0;

static void	handle_sigint_parent(int signum)
{
	(void)signum;
	rl_on_new_line();
	rl_replace_line("", 0);
	g_signal_status = 130;
}

// static void	handle_after_child(int signum, t_shell *shell)
// {
// 	shell->exit_code = signum;
// 	printf("\n");
// }

void	setup_parent_signals(void)
{
	signal(SIGINT, handle_sigint_parent);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void handle_sigint_heredoc(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 06:09:15 by amal              #+#    #+#             */
/*   Updated: 2025/06/21 12:21:05 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	g_signal_status = 0;

void	handle_sigint_parent(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal_status = 130;
}
void	handle_sigquit_parent(int signum)
{
	(void)signum;
	write(1, "Quit (core dumped)\n", 20);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal_status = 131;
}
void	setup_parent_signals(void)
{
	signal(SIGINT, handle_sigint_parent);
	signal(SIGQUIT, handle_sigquit_parent);
}
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
static void	handle_sigint_heredoc(int signum)
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

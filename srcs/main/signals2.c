/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:42:18 by amrashid          #+#    #+#             */
/*   Updated: 2025/06/21 12:42:26 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

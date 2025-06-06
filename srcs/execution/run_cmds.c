/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:23:25 by amal              #+#    #+#             */
/*   Updated: 2025/06/06 21:26:22 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_signal(int status, t_shell *shell)
{
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit: 3\n", 8);
	}
}

void	run_cmds(t_shell *shell)
{
	int	status;

	if (!shell->cmds)
		return ;
	if (is_builtin_cmd(shell->cmds) && !shell->cmds->next)
		shell->exit_code = exec_builtin_single(shell, STDIN_FILENO, STDOUT_FILENO);
	else
		exec_cmds(shell, STDIN_FILENO, STDOUT_FILENO);
	if (shell->last_pid > 0)
	{
		while (waitpid(-1, &status, 0) != -1)
			handle_signal(status, shell);
	}
	cleanup_heredoc(shell->cmds);
}


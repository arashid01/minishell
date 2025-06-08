/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 09:58:28 by nora              #+#    #+#             */
/*   Updated: 2025/06/08 15:57:24 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_exit_args(t_shell *shell, long long *exit_code)
{
	int	status;

	*exit_code = ft_atolli(shell->cmds->args[1], &status);
	if (status != 0)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(shell->cmds->args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		return (255);
	}
	if (shell->cmds->args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		shell->exit_code = 1;
		return (-1);
	}
	return (0);
}

void	ft_exit(t_shell *shell)
{
	long long	code;
	int			result;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (!shell->cmds || !shell->cmds->args || !shell->cmds->args[1])
	{
		free_cmds(shell->cmds);
		exit(shell->exit_code);
	}
	result = handle_exit_args(shell, &code);
	if (result == -1)
		return ;
	if (result == 255)
	{
		free_cmds(shell->cmds);
		exit(255);
	}
	free_cmds(shell->cmds);
	exit((unsigned char)code);
}

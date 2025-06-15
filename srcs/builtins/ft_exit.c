/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 09:58:28 by nora              #+#    #+#             */
/*   Updated: 2025/06/13 00:10:51 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_exit_args(t_shell *shell, t_cmd *cmds, long long *exit_code)
{
	int	status;

	*exit_code = ft_atolli(cmds->args[1], &status);
	if (status != 0)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmds->args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		return (255);
	}
	if (cmds->args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		shell->exit_code = 1;
		return (-1);
	}
	return (0);
}

void	ft_exit(t_shell *shell, t_cmd *cmds)
{
	long long	code;
	int			result;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (!cmds || !cmds->args || !cmds->args[1])
	{
		free_cmds(cmds);
		exit(shell->exit_code);
	}
	result = handle_exit_args(shell, cmds, &code);
	if (result == -1)
		return ;
	if (result == 255)
	{
		free_cmds(cmds);
		exit(255);
	}
	free_cmds(cmds);
	exit((unsigned char)code);
}

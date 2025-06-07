/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 09:58:28 by nora              #+#    #+#             */
/*   Updated: 2025/06/07 19:31:12 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_exit_args(t_cmd *cmd, long long *exit_code)
{
	int	status;

	*exit_code = ft_atolli(cmd->args[1], &status);
	if (status != 0)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		return (255);
	}
	if (cmd->args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		g_exit_status = 1;
		return (-1);
	}
	return (0);
}

void	ft_exit(t_cmd *cmd)
{
	long long	code;
	int			result;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (!cmd || !cmd->args || !cmd->args[1])
	{
		free_cmds(cmd);
		exit(g_exit_status);
	}
	result = handle_exit_args(cmd, &code);
	if (result == -1)
		return ;
	if (result == 255)
	{
		free_cmds(cmd);
		exit(255);
	}
	free_cmds(cmd);
	exit((unsigned char)code);
}

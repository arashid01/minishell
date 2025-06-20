/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 09:58:28 by nora              #+#    #+#             */
/*   Updated: 2025/06/20 14:28:35 by amrashid         ###   ########.fr       */
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
		return (2);
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
	int			i;
	int			result;
	long long	code;

	ft_putendl_fd("exit", STDERR_FILENO);
	close(shell->std_out);
	if (!cmds || !cmds->args || !cmds->args[1])
	{
		i = shell->exit_code;
		free_arr(shell->argv);
		free_cmds(cmds);
		free(shell);
		exit(i);
	}
	result = handle_exit_args(shell, cmds, &code);
	if (result == -1)
	{
		// printf("im here\n");
		return ;
	}
	if (result == 2)
	{
		free_arr(shell->argv);
		free_cmds(cmds);
		free(shell);
		exit(2);
	}
	free_arr(shell->argv);
	free_cmds(cmds);
	free(shell);
	exit((unsigned char)code);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:00:00 by nora              #+#    #+#             */
/*   Updated: 2025/06/05 14:54:59 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_export(t_shell *shell)
{
	int i;
	int status;

	if (!shell->cmds || !shell->cmds->args)
	{
		write(STDERR_FILENO, "minishell: export: invalid input\n", 33);
		return (1);
	}
	if (!shell->env || !*shell->env) //check *shell->env
	{
		write(STDERR_FILENO, "minishell: export: environment not available\n", 45);
		return (1);
	}
	if (shell->cmds->args[1] == NULL)
		return (export_display(shell->env));
	else
	{
		i = 1;
		status = 0;
		while (shell->cmds->args[i])
		{
			if (process_exp_arg(shell->cmds->args[i], &shell->env) != 0) //check char **env
				status = 1;
			i++;
		}
		return (status);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:00:00 by nora              #+#    #+#             */
/*   Updated: 2025/06/13 00:11:37 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_export_args(char **args, char ***env)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (args[i])
	{
		if (process_exp_arg(args[i], env) != 0)
			status = 1;
		i++;
	}
	return (status);
}

int	ft_export(t_shell *shell, t_cmd *cmds)
{
	if (!cmds || !cmds->args)
	{
		write(STDERR_FILENO, "minishell: export: invalid input\n", 33);
		return (1);
	}
	if (!shell->env || !*shell->env)
	{
		write(STDERR_FILENO, "minishell: export: environment not available\n",
			45);
		return (1);
	}
	if (cmds->args[1] == NULL)
		return (export_display(shell->env));
	return (handle_export_args(cmds->args, &shell->env));
}

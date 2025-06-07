/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:00:00 by nora              #+#    #+#             */
/*   Updated: 2025/06/07 19:49:56 by nora             ###   ########.fr       */
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

int	ft_export(t_cmd *cmd, char ***env)
{
	if (!cmd || !cmd->args)
		return (write(STDERR_FILENO, "minishell: export: invalid input\n",
				33), 1);
	if (!env || !*env)
		return (write(STDERR_FILENO,
				"minishell: export: environment not available\n", 45), 1);
	if (!cmd->args[1])
		return (export_display(*env));
	return (handle_export_args(cmd->args, env));
}

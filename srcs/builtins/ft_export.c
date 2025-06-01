/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:00:00 by nora              #+#    #+#             */
/*   Updated: 2025/06/01 10:55:56 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_export(t_cmd *cmd, char ***env)
{
	int i;
	int status;

	if (!cmd || !cmd->args)
	{
		write(STDERR_FILENO, "minishell: export: invalid input\n", 33);
		return (1);
	}
	if (!env || !*env)
	{
		write(STDERR_FILENO, "minishell: export: environment not available\n", 45);
		return (1);
	}
	if (cmd->args[1] == NULL)
		return (exp_display_mode(*env));
	else
	{
		i = 1;
		status = 0;
		while (cmd->args[i])
		{
			if (process_exp_arg(cmd->args[i], env) != 0)
				status = 1;
			i++;
		}
		return (status);
	}
}

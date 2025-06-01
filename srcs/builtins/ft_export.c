/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:00:00 by nora              #+#    #+#             */
/*   Updated: 2025/05/25 23:01:44 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_export(t_cmd *cmd, char ***envp_ptr)
{
	int i;
	int status;

	if (!cmd || !cmd->args)
	{
		write(STDERR_FILENO, "minishell: export: invalid input\n", 33);
		return (1);
	}
	if (!envp_ptr || !*envp_ptr)
	{
		write(STDERR_FILENO, "minishell: export: environment not available\n", 45);
		return (1);
	}
	if (cmd->args[1] == NULL)
		return (exp_display_mode(*envp_ptr));
	else
	{
		i = 1;
		status = 0;
		while (cmd->args[i])
		{
			if (process_exp_arg(cmd->args[i], envp_ptr) != 0)
				status = 1;
			i++;
		}
		return (status);
	}
}

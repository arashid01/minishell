/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:59:18 by nora              #+#    #+#             */
/*   Updated: 2025/05/27 02:59:43 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_unset(t_cmd *cmd, char ***envp_ptr)
{
	int i;
	int status;

	if (!cmd || !cmd->args)
	{
		write(STDERR_FILENO, "minishell: unset: invalid input\n", 33);
		return (1);
	}
	if (!cmd->args[1])
		return (0);
	i = 1;
	status = 0;
	while (cmd->args[i])
	{
		if (!is_valid_name(cmd->args[i]))
		{
			 write(STDERR_FILENO, "minishell: unset: '", 19);
			 write(STDERR_FILENO, cmd->args[i], ft_strlen(cmd->args[i]));
			 write(STDERR_FILENO, "': not a valid identifier\n", 26);
			 status = 1;
		}
		else
			ft_unsetenv(cmd->args[i], envp_ptr);
		i++;
	}
	return (status);
}

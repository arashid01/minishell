/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:59:18 by nora              #+#    #+#             */
/*   Updated: 2025/06/06 03:16:24 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_unset(t_shell *shell)
{
	int i;
	int status;

	if (!shell->cmds || !shell->cmds->args)
	{
		write(STDERR_FILENO, "minishell: unset: invalid input\n", 33);
		return (1);
	}
	if (!shell->cmds->args[1])
		return (0);
	i = 1;
	status = 0;
	while (shell->cmds->args[i])
	{
		if (!is_valid_name(shell->cmds->args[i]))
		{
			 write(STDERR_FILENO, "minishell: unset: '", 19);
			 write(STDERR_FILENO, shell->cmds->args[i], ft_strlen(shell->cmds->args[i]));
			 write(STDERR_FILENO, "': not a valid identifier\n", 26);
			 status = 1;
		}
		else
			ft_unsetenv(shell->cmds->args[i], &shell->env);
		i++;
	}
	return (status);
}

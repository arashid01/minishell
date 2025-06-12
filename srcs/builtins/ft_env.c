/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:50:26 by nora              #+#    #+#             */
/*   Updated: 2025/06/13 00:08:53 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(t_shell *shell, t_cmd *cmds)
{
	int	i;

	if (!cmds || !cmds->args
		|| cmds->args[1] != NULL || !shell->env)
	{
		if (!cmds || !cmds->args)
			write(2, "minishell: env: invalid input\n", 30);
		else if (cmds->args[1] != NULL)
			write(2, "minishell: env: too many arguments\n", 35);
		else
			write(2, "minishell: env: environment not available\n", 42);
		return (1);
	}
	i = 0;
	while (shell->env[i])
	{
		if (ft_strchr(shell->env[i], '='))
		{
			write(1, shell->env[i], ft_strlen(shell->env[i]));
			write(1, "\n", 1);
		}
		i++;
	}
	return (0);
}

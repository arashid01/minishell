/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:50:26 by nora              #+#    #+#             */
/*   Updated: 2025/06/08 13:41:53 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(t_shell *shell)
{
	int	i;

	if (!shell->cmds || !shell->cmds->args
		|| shell->cmds->args[1] != NULL || !shell->env)
	{
		if (!shell->cmds || !shell->cmds->args)
			write(2, "minishell: env: invalid input\n", 30);
		else if (shell->cmds->args[1] != NULL)
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:50:26 by nora              #+#    #+#             */
/*   Updated: 2025/06/07 18:53:17 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(t_cmd *cmd, char **env_arr)
{
	int	i;

	if (!cmd || !cmd->args || cmd->args[1] != NULL || !env_arr)
	{
		if (!cmd || !cmd->args)
			write(2, "minishell: env: invalid input\n", 30);
		else if (cmd->args[1] != NULL)
			write(2, "minishell: env: too many arguments\n", 35);
		else
			write(2, "minishell: env: environment not available\n", 42);
		return (1);
	}
	i = 0;
	while (env_arr[i])
	{
		if (ft_strchr(env_arr[i], '='))
		{
			write(1, env_arr[i], ft_strlen(env_arr[i]));
			write(1, "\n", 1);
		}
		i++;
	}
	return (0);
}

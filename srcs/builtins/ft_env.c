/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:50:26 by nora              #+#    #+#             */
/*   Updated: 2025/05/25 08:27:42 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(t_cmd *cmd, char **envp_arr)
{
	int	i;

	if (!cmd || !cmd->args)
	{
		write(2, "minishell: env: invalid input\n", 30);
		return (1);
	}
	if (cmd->args[1] != NULL)
	{
		write(2, "minishell: env: too many arguments\n", 35);
		return (1);
	}
	if (!envp_arr)
	{
		write(2, "minishell: env: environment not available\n", 42);
		return (1);
	}
	i = 0;
	while (envp_arr[i])
	{
		if (ft_strchr(envp_arr[i], '=') != NULL)
		{
			write(1, envp_arr[i], ft_strlen(envp_arr[i]));
			write(1, "\n", 1);
		}
		i++;
	}
	return (0);
}

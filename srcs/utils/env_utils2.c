/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:28:14 by amal              #+#    #+#             */
/*   Updated: 2025/06/08 13:26:01 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_env_args(const char *name, const char *value, char ***env)
{
	if (!name || !env || !*env)
	{
		write(STDERR_FILENO, "minishell: env: invalid arguments\n", 34);
		return (1);
	}
	if (value && ft_strchr(name, '='))
	{
		write(STDERR_FILENO, "minishell: setenv: invalid arguments\n", 38);
		return (1);
	}
	return (0);
}

int	find_env_idx(const char *name, char **envp)
{
	int		i;
	size_t	name_len;

	name_len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, name_len) == 0
			&& envp[i][name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

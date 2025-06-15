/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:39:53 by amal              #+#    #+#             */
/*   Updated: 2025/06/15 15:45:05 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	fill_env_copy(char **dst, char **src, int count)
{
	int	i;

	i = 0;
	(void)count;
	while (src[i])
	{
		dst[i] = ft_strdup(src[i]);
		if (!dst[i])
		{
			free_arr(dst);
			perror("minishell: copy_env: malloc failed");
			return (1);
		}
		i++;
	}
	dst[i] = NULL;
	return (0);
}

char	**copy_env(char **envp)
{
	int		count;
	char	**envp_copy;

	if (!envp)
		return (NULL);
	count = 0;
	while (envp[count])
		count++;
	envp_copy = malloc(sizeof(char *) * (count + 1));
	if (!envp_copy)
	{
		perror("minishell: copy_env: malloc failed");
		return (NULL);
	}
	if (fill_env_copy(envp_copy, envp, count))
		return (NULL);
	return (envp_copy);
}

char	*get_env_val(char **env_arr, const char *var)
{
	int		i;
	int		var_len;
	char	*value;

	if (!var || !env_arr)
		return (NULL);
	i = 0;
	var_len = ft_strlen(var);
	while (env_arr[i])
	{
		if ((ft_strncmp(env_arr[i], var, var_len) == 0)
			&& (env_arr[i][var_len] == '='))
		{
			value = ft_strchr(env_arr[i], '=') + 1;
			return (ft_strdup(value));
		}
		i++;
	}
	return (NULL);
}

int	print_sorted_env(char **sorted_env)
{
	int	i;

	i = 0;
	while (sorted_env[i])
	{
		print_exp_var(sorted_env[i]);
		i++;
	}
	return (0);
}

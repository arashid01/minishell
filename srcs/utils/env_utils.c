/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:39:53 by amal              #+#    #+#             */
/*   Updated: 2025/05/27 02:42:34 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char **copy_env(char **envp)
{
	int		i;
	int		count;
	char	**envp_copy;

	i = 0;
	count = 0;
	if (!envp)
		return (NULL);
	while (envp[count])
		count++;
	envp_copy = malloc(sizeof(char *) * (count + 1));
	if (!envp_copy)
	{
		perror("minishell: malloc failed");
		return (NULL);
	}
	while (i < count)
	{
		envp_copy[i] = ft_strdup(envp[i]);
		if (!envp_copy[i])
		{
			free_arr(envp_copy);
			perror("minishell: malloc failed");
			return (NULL);
		}
		i++;
	}
	envp_copy[count] = NULL;
	return (envp_copy);
}

char	*get_env_value(char **env_array, const char *var)
{
	int 	i;
	int 	var_len;
	char	*value;

	if (!var || !env_array)
		return (NULL);
	i = 0;
	var_len = ft_strlen(var);
	while (env_array[i])
	{
		if ((ft_strncmp(env_array[i], var, var_len) == 0) 
			&& (env_array[i][var_len] == '='))
		{
			value = ft_strchr(env_array[i], '=') + 1;
			return ft_strdup(value);
		}
		i++;
	}
	return (NULL);
}

void free_env(char **envp)
{
	if (!envp)
		return;
	free_arr(envp);
}

int	print_sorted_env(char **sorted_env)
{
	int i;

	i = 0;
	while (sorted_env[i])
	{
		print_exp_var(sorted_env[i]);
		i++;
	}
	return (0);
}

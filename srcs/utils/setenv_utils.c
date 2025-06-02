/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 02:42:47 by amal              #+#    #+#             */
/*   Updated: 2025/06/02 03:46:34 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*create_env_entry(const char *name, const char *value)
{
	size_t	name_len;
	size_t	value_len;
	char	*entry;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	entry = malloc(name_len + value_len + 2);
	if (!entry)
	{
		perror("minishell: ft_setenv: malloc failed");
		return (NULL);
	}
	ft_strlcpy(entry, name, name_len + 1);
	entry[name_len] = '=';
	ft_strlcpy(entry + name_len + 1, value, value_len + 1);
	return (entry);
}

static int	update_env_var(char ***env, int idx, const char *name, const char *value)
{
	char	*new_entry;

	new_entry = create_env_entry(name, value);
	if (!new_entry)
		return (1);
	free((*env)[idx]);
	(*env)[idx] = new_entry;
	return (0);
}

static int	add_env_var(char ***env, const char *name, const char *value)
{
	int		env_count;
	int		i;
	char	**new_envp;
	char	*new_entry;

	env_count = 0;
	while ((*env)[env_count])
		env_count++;
	new_envp = malloc(sizeof(char *) * (env_count + 2));
	if (!new_envp)
	{
		perror("minishell: ft_setenv: malloc failed");
		return (1);
	}
	new_entry = create_env_entry(name, value);
	if (!new_entry)
	{
		free(new_envp);
		return (1);
	}
	i = 0;
	while (i < env_count)
	{
		new_envp[i] = (*env)[i];
		i++;
	}
	new_envp[env_count] = new_entry;
	new_envp[env_count + 1] = NULL;
	free(*env);
	*env = new_envp;
	return (0);
}

int	ft_setenv(const char *name, const char *value, char ***env)
{
	int	idx;

	if (check_env_args(name, value, env))
		return (1);
	idx = find_env_idx(name, *env);
	if (idx >= 0)
		return (update_env_var(env, idx, name, value));
	else
		return (add_env_var(env, name, value));
}

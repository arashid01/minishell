/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 02:42:47 by amal              #+#    #+#             */
/*   Updated: 2025/06/15 13:49:14 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*create_env_entry(const char *name, const char *value)
{
	size_t	name_len;
	size_t	value_len;
	char	*entry;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	printf("Name: %s Value: %s\n", name, value);
	entry = malloc(name_len + value_len + 2);
	if (!entry)
	{
		perror("minishell: ft_setenv: malloc failed");
		return (NULL);
	}
	ft_strlcpy(entry, name, name_len + 1);
	if (*value)
	{
		entry[name_len] = '=';
		ft_strlcpy(entry + name_len + 1, value, value_len + 1);
	}
	return (entry);
}

static int	update_env_var(char ***env, int idx,
		const char *name, const char *value)
{
	char	*new_entry;

	new_entry = create_env_entry(name, value);
	if (!new_entry)
		return (1);
	free((*env)[idx]);
	(*env)[idx] = new_entry;
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

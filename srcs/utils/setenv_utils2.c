/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 13:30:50 by nora              #+#    #+#             */
/*   Updated: 2025/06/12 13:46:31 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	copy_old_env(char **new_envp, char **old_env, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_envp[i] = old_env[i];
		i++;
	}
}

static char	**alloc_env_with_new_entry(char ***env, int *env_count)
{
	char	**new_envp;

	*env_count = 0;
	while ((*env)[*env_count])
		(*env_count)++;
	new_envp = malloc(sizeof(char *) * (*env_count + 2));
	if (!new_envp)
		perror("minishell: ft_setenv: malloc failed");
	return (new_envp);
}

int	add_env_var(char ***env, const char *name, const char *value)
{
	int		count;
	char	**new_envp;
	char	*new_entry;

	new_envp = alloc_env_with_new_entry(env, &count);
	if (!new_envp)
		return (1);
	new_entry = create_env_entry(name, value);
	if (!new_entry)
	{
		free(new_envp);
		return (1);
	}
	copy_old_env(new_envp, *env, count);
	new_envp[count] = new_entry;
	new_envp[count + 1] = NULL;
	free(*env);
	*env = new_envp;
	return (0);
}

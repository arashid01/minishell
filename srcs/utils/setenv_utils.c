/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 02:42:47 by amal              #+#    #+#             */
/*   Updated: 2025/06/01 10:55:56 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_setenv(const char *name, const char *value, char ***env)
{
	size_t	name_len;
	int		i;
	int		env_count;
	char	*new_entry;
	char	**new_envp;

	if (!name || !value || !env || !*env || ft_strchr(name, '='))
	{
		write(STDERR_FILENO, "minishell: ft_setenv: invalid arguments\n", 40);
		return (1);
	}
	name_len = ft_strlen(name);
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], name, name_len) == 0 && (*env)[i][name_len] == '=')
		{
		   
			new_entry = malloc(name_len + ft_strlen(value) + 2);
			if (!new_entry)
			{
				perror("minishell: ft_setenv: malloc failed");
				return (1);
			}
			ft_strlcpy(new_entry, name, name_len + 1);
			new_entry[name_len] = '=';
			ft_strlcpy(new_entry + name_len + 1, value, ft_strlen(value) + 1);
			free((*env)[i]);
			(*env)[i] = new_entry;
			return (0);
		}
		i++;
	}
	env_count = i;
	new_envp = malloc(sizeof(char *) * (env_count + 2));
	if (!new_envp)
	{
		perror("minishell: ft_setenv: malloc failed");
		return (1);
	}
	new_entry = malloc(name_len + ft_strlen(value) + 2);
	if (!new_entry)
	{
		free(new_envp);
		perror("minishell: ft_setenv: malloc failed");
		return (1);
	}
	ft_strlcpy(new_entry, name, name_len + 1);
	new_entry[name_len] = '=';
	ft_strlcpy(new_entry + name_len + 1, value, ft_strlen(value) + 1);
	for (i = 0; i < env_count; i++)
		new_envp[i] = (*env)[i];
	new_envp[env_count] = new_entry;
	new_envp[env_count + 1] = NULL;
	free(*env);
	*env = new_envp;
	return (0);
}
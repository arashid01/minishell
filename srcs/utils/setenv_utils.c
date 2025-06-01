/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 02:42:47 by amal              #+#    #+#             */
/*   Updated: 2025/05/27 02:44:49 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_setenv(const char *name, const char *value, char ***envp_ptr)
{
	size_t	name_len;
	int		i;
	int		env_count;
	char	*new_entry;
	char	**new_envp;

	if (!name || !value || !envp_ptr || !*envp_ptr || ft_strchr(name, '='))
	{
		write(STDERR_FILENO, "minishell: ft_setenv: invalid arguments\n", 40);
		return (1);
	}
	name_len = ft_strlen(name);
	i = 0;
	while ((*envp_ptr)[i])
	{
		if (ft_strncmp((*envp_ptr)[i], name, name_len) == 0 && (*envp_ptr)[i][name_len] == '=')
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
			free((*envp_ptr)[i]);
			(*envp_ptr)[i] = new_entry;
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
		new_envp[i] = (*envp_ptr)[i];
	new_envp[env_count] = new_entry;
	new_envp[env_count + 1] = NULL;
	free(*envp_ptr);
	*envp_ptr = new_envp;
	return (0);
}
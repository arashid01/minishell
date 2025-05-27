/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 02:45:19 by amal              #+#    #+#             */
/*   Updated: 2025/05/27 02:46:47 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_unsetenv(const char *name, char ***envp_ptr)
{
	int		i;
	int		j;
	int		old_count;
	char	**old_envp;
	char	**new_envp;
	size_t	name_len;
	int		found_index;

	if (!name || !envp_ptr || !*envp_ptr)
		return (1);
	old_envp = *envp_ptr;
	name_len = ft_strlen(name);
	old_count = 0;
	found_index = -1;
	while (old_envp[old_count])
	{
	   
		if (found_index == -1 && ft_strncmp(old_envp[old_count], name, name_len) == 0 && old_envp[old_count][name_len] == '=')
		{
			found_index = old_count;
		}
		old_count++;
	}
	if (found_index == -1)
		return (0);
	new_envp = malloc(sizeof(char *) * old_count);
	if (!new_envp)
	{
		perror("minishell: ft_unsetenv: malloc failed");
		return (1);
	}
	j = 0;
	for (i = 0; i < old_count; i++)
	{
		if (i == found_index)
		{
			free(old_envp[i]);
			continue;
		}
		new_envp[j++] = old_envp[i];
	}
	new_envp[j] = NULL;
	free(old_envp);
	*envp_ptr = new_envp;
	return (0);
}
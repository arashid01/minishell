/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 02:45:19 by amal              #+#    #+#             */
/*   Updated: 2025/06/12 13:46:31 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**create_new_envp(char **old_envp, int old_count, int skip_idx)
{
	char	**new_envp;
	int		i;
	int		j;

	new_envp = malloc(sizeof(char *) * old_count);
	if (!new_envp)
	{
		perror("minishell: ft_unsetenv: malloc failed");
		return (NULL);
	}
	i = 0;
	j = 0;
	while (i < old_count)
	{
		if (i == skip_idx)
		{
			free(old_envp[i]);
			i++;
			continue ;
		}
		new_envp[j++] = old_envp[i++];
	}
	new_envp[j] = NULL;
	return (new_envp);
}

int	ft_unsetenv(const char *name, char ***env)
{
	int		old_count;
	int		found_idx;
	char	**new_envp;
	char	**old_envp;

	if (check_env_args(name, NULL, env))
		return (1);
	old_envp = *env;
	old_count = 0;
	while (old_envp[old_count])
		old_count++;
	found_idx = find_env_idx(name, old_envp);
	if (found_idx == -1)
		return (0);
	new_envp = create_new_envp(old_envp, old_count, found_idx);
	if (!new_envp)
		return (1);
	free(old_envp);
	*env = new_envp;
	return (0);
}

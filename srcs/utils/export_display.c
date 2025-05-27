/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:55:45 by amal              #+#    #+#             */
/*   Updated: 2025/05/27 19:10:15 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char **ft_copy_str_array(char **arr)
{
	char	**copied_arr;
	int		i;
	int		count;

	if (!arr)
		return (NULL);
	count = 0;
	while (arr[count])
		count++;
	copied_arr = malloc(sizeof(char *) * (count + 1));
	if (!copied_arr)
	{
		perror("minishell: malloc failed");
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		copied_arr[i] = ft_strdup(arr[i]);
		if (!copied_arr[i])
		{
		   
			free_arr(copied_arr);
			perror("minishell: malloc failed");
			return (NULL);
		}
		i++;
	}
	copied_arr[count] = NULL;
	return (copied_arr);
}

int exp_display_mode(char **envp_arr)
{
	char	**sorted_env;
	int		status;

	sorted_env = ft_copy_str_array(envp_arr);
	if (!sorted_env)
		return (1);
	ft_sort_string_array(sorted_env);
	status = print_sorted_env(sorted_env);
	free_arr(sorted_env);
	return (status);
}

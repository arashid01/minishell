/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:55:45 by amal              #+#    #+#             */
/*   Updated: 2025/06/12 13:46:31 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	export_display(char **env_arr)
{
	char	**sorted_env;
	int		status;

	sorted_env = ft_copy_str_arr(env_arr);
	if (!sorted_env)
		return (1);
	ft_sort_str_arr(sorted_env);
	status = print_sorted_env(sorted_env);
	free_arr(sorted_env);
	return (status);
}

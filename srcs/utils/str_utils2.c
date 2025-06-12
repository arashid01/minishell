/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 13:25:00 by nora              #+#    #+#             */
/*   Updated: 2025/06/12 13:46:31 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_arr(char **arr)
{
	int	count;

	count = 0;
	while (arr[count])
		count++;
	return (count);
}

static char	**handle_dup_failure(char **copy)
{
	free_arr(copy);
	perror("minishell: malloc failed");
	return (NULL);
}

char	**ft_copy_str_arr(char **arr)
{
	char	**copy;
	int		i;
	int		count;

	if (!arr)
		return (NULL);
	count = count_arr(arr);
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = -1;
	while (++i < count)
	{
		copy[i] = ft_strdup(arr[i]);
		if (!copy[i])
			return (handle_dup_failure(copy));
	}
	copy[count] = NULL;
	return (copy);
}

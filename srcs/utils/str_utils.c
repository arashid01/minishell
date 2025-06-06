/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 22:59:05 by amal              #+#    #+#             */
/*   Updated: 2025/06/06 02:58:25 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_swap_str(char **s1, char **s2)
{
	char *temp;

	temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}

void	ft_sort_str_arr(char **arr)
{
	int i;
	int j;
	int swapped;
	int len;

	if (!arr || !*arr)
		return ;
	len = 0;
	while (arr[len] != NULL)
		len++;
	i = 0;
	while (i < len - 1)
	{
		swapped = 0;
		j = 0;
		while (j < len - 1 - i)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				ft_swap_str(&arr[j], &arr[j + 1]);
				swapped = 1;
			}
			j++;
		}
		if (swapped == 0)
			break;
		i++;
	}
}

char	**ft_copy_str_arr(char **arr)
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

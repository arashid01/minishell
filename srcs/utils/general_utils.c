/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 22:59:05 by amal              #+#    #+#             */
/*   Updated: 2025/06/02 06:49:20 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_error(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

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

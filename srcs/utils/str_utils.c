/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 22:59:05 by amal              #+#    #+#             */
/*   Updated: 2025/06/08 13:24:37 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_swap_str(char **s1, char **s2)
{
	char	*temp;

	temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}

static void	sort_pass(char **arr, int len, int i, int *swapped)
{
	int	j;

	j = 0;
	while (j < len - 1 - i)
	{
		if (ft_strcmp(arr[j], arr[j + 1]) > 0)
		{
			ft_swap_str(&arr[j], &arr[j + 1]);
			*swapped = 1;
		}
		j++;
	}
}

void	ft_sort_str_arr(char **arr)
{
	int	i;
	int	swapped;
	int	len;

	if (!arr || !*arr)
		return ;
	len = 0;
	while (arr[len])
		len++;
	i = 0;
	while (i < len - 1)
	{
		swapped = 0;
		sort_pass(arr, len, i, &swapped);
		if (!swapped)
			break ;
		i++;
	}
}

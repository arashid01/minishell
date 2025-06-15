/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:55:52 by nora              #+#    #+#             */
/*   Updated: 2025/06/08 15:57:13 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

static int	skip_ws_sign(const char *str, int *i, int *sign_val)
{
	*sign_val = 1;
	while (ft_isspace(str[*i]))
		(*i)++;
	if (str[*i] == '-')
	{
		*sign_val = -1;
		(*i)++;
	}
	else if (str[*i] == '+')
	{
		*sign_val = 1;
		(*i)++;
	}
	if (!ft_isdigit(str[*i]))
		return (1);
	return (0);
}

static int	parse_num_ovf(const char *str, int *i, long long *value_accumulated)
{
	long long	prev;

	*value_accumulated = 0;
	while (ft_isdigit(str[*i]))
	{
		prev = *value_accumulated;
		*value_accumulated = (*value_accumulated) * 10 + (str[*i] - '0');
		if (((*value_accumulated) / 10) != prev)
			return (1);
		(*i)++;
	}
	return (0);
}

long long	ft_atolli(const char *str, int *status)
{
	int			i;
	int			sign_val;
	long long	value_accumulated;

	i = 0;
	if (status)
		*status = 0;
	if (skip_ws_sign(str, &i, &sign_val) != 0)
	{
		if (status)
			*status = 2;
		return (0);
	}
	if (parse_num_ovf(str, &i, &value_accumulated) != 0)
	{
		if (status)
			*status = 1;
		return (0);
	}
	if (str[i] != '\0')
	{
		if (status)
			*status = 2;
	}
	return (value_accumulated * sign_val);
}

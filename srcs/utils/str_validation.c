/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:48:58 by amal              #+#    #+#             */
/*   Updated: 2025/06/08 13:27:40 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	is_whitespace_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!is_whitespace(line[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_char(int c)
{
	return (ft_isalnum(c) || c == '_');
}

int	is_valid_name(const char *name)
{
	int	i;

	if (!name || name[0] == '\0')
		return (0);
	if (!ft_isalpha((unsigned char)name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i] && name[i] != '=')
	{
		if (!is_valid_char((unsigned char)name[i]))
			return (0);
		i++;
	}
	return (1);
}

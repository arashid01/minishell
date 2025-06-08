/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 21:21:17 by nora              #+#    #+#             */
/*   Updated: 2025/06/08 16:14:03 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_n_flag(char **args, int *index)
{
	int	j;

	while (args[*index] && ft_strncmp(args[*index], "-n", 2) == 0)
	{
		j = 2;
		while (args[*index][j] == 'n')
			j++;
		if (args[*index][j] != '\0')
			break ;
		(*index)++;
	}
	return (*index);
}

int	ft_echo(t_cmd *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	i = check_n_flag(cmd->args, &i);
	if (i > 1)
		newline = 0;
	while (cmd->args[i])
	{
		write(1, cmd->args[i], ft_strlen(cmd->args[i]));
		if (cmd->args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}

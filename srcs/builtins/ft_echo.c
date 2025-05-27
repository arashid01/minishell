/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 21:21:17 by nora              #+#    #+#             */
/*   Updated: 2025/05/25 08:23:51 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_echo(t_cmd *cmd)
{
	int		i;
	int		newline;

	i = 1;
	newline = 1;
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "-n", 2) == 0)
	{
		int j = 2;
		while (cmd->args[i][j] == 'n')
			j++;
		if (cmd->args[i][j] != '\0')
			break;
		newline = 0;
		i++;
	}
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:07:50 by nora              #+#    #+#             */
/*   Updated: 2025/06/08 13:58:43 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_pwd(t_cmd *cmd)
{
	char	buffer[1024];

	if (!cmd || !cmd->args)
	{
		write(2, "minishell: pwd: invalid input\n", 30);
		return (1);
	}
	if (cmd->args[1] != NULL)
	{
		write(2, "minishell: pwd: too many arguments\n", 35);
		return (1);
	}
	if (getcwd(buffer, sizeof(buffer)) == NULL)
	{
		perror("minishell: pwd: getcwd");
		return (1);
	}
	write(1, buffer, ft_strlen(buffer));
	write(1, "\n", 1);
	return (0);
}

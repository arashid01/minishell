/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:59:18 by nora              #+#    #+#             */
/*   Updated: 2025/06/07 19:46:25 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#include "../../includes/minishell.h"

static int	unset_var(const char *arg, char ***env)
{
	if (!is_valid_name(arg))
	{
		write(STDERR_FILENO, "minishell: unset: '", 19);
		write(STDERR_FILENO, arg, ft_strlen(arg));
		write(STDERR_FILENO, "': not a valid identifier\n", 26);
		return (1);
	}
	ft_unsetenv(arg, env);
	return (0);
}

int	ft_unset(t_cmd *cmd, char ***env)
{
	int	i;
	int	status;

	if (!cmd || !cmd->args)
	{
		write(STDERR_FILENO, "minishell: unset: invalid input\n", 33);
		return (1);
	}
	i = 1;
	status = 0;
	while (cmd->args[i])
		status |= unset_var(cmd->args[i++], env);
	return (status);
}

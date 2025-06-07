/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 21:17:58 by nora              #+#    #+#             */
/*   Updated: 2025/06/07 19:36:25 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_arg(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
		i++;
	return (i);
}

int	check_cd_args(t_cmd *cmd)
{
	int	argc;

	if (!cmd || !cmd->args)
	{
		write(2, "minishell: cd: invalid input\n", 29);
		return (1);
	}
	argc = count_arg(cmd->args);
	if (argc > 2)
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		return (1);
	}
	return (0);
}

char	*get_target_path(t_cmd *cmd, char ***env)
{
	char	*target_path;

	if (!cmd->args[1])
	{
		target_path = get_env_val(*env, "HOME");
		if (!target_path)
			write(2, "minishell: cd: HOME not set\n", 28);
	}
	else
	{
		target_path = ft_strdup(cmd->args[1]);
		if (!target_path)
			ft_error("minishell: cd: malloc failed for target path");
	}
	return (target_path);
}

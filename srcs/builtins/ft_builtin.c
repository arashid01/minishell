/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 11:21:04 by nora              #+#    #+#             */
/*   Updated: 2025/05/27 03:34:49 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin_cmd(t_cmd *cmd)
{
	int		i;
	char	*builtins[7];

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(cmd->args[0], builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	exec_builtin(t_cmd *cmd, char ***envp_ptr)
{
	int	status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	status = 1;
	if (ft_strcmp(cmd->args[0], "echo") == 0) 
		status = ft_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0) 
		status = ft_cd(cmd, envp_ptr); 
	else if (ft_strcmp(cmd->args[0], "pwd") == 0) 
		status = ft_pwd(cmd);
	else if (ft_strcmp(cmd->args[0], "export") == 0) 
		status = ft_export(cmd, envp_ptr); 
	else if (ft_strcmp(cmd->args[0], "env") == 0) 
		status = ft_env(cmd, *envp_ptr); 
	else if (ft_strcmp(cmd->args[0], "unset") == 0) 
		status = ft_unset(cmd, envp_ptr); 
	else if (ft_strcmp(cmd->args[0], "exit") == 0) 
	{
		ft_exit(cmd); 
		return (g_exit_status); 
	}
	g_exit_status = status;
	return (status);
}

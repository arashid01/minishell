/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 21:17:58 by nora              #+#    #+#             */
/*   Updated: 2025/06/06 03:15:58 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_cd_args(t_cmd *cmd)
{
	if (!cmd || !cmd->args)
	{
		write(2, "minishell: cd: invalid input\n", 29);
		return (1);
	}
	if (!cmd->args[1])
	{
		write(2, "minishell: cd: missing argument\n", 32);
		return (1);
	}
	if (cmd->args[2])
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		return (1);
	}
	return (0);
}

static char	*get_old_pwd(void)
{
	char	buffer[1024];

	if (!getcwd(buffer, sizeof(buffer)))
	{
		perror("minishell: cd: getcwd");
		return (NULL);
	}
	return (ft_strdup(buffer));
}

static int	try_chdir(char *target)
{
	if (chdir(target) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

static int	update_pwd_env(char ***env, char *old_pwd)
{
	char	buffer[1024];
	char	*new_pwd;
	int		status;

	status = 0;
	if (ft_setenv("OLDPWD", old_pwd, env) != 0)
		status = 1;
	if (!getcwd(buffer, sizeof(buffer)))
	{
		perror("minishell: cd: getcwd after chdir");
		if (ft_unsetenv("PWD", env) != 0)
			status = 1;
		return (status);
	}
	new_pwd = ft_strdup(buffer);
	if (!new_pwd)
	{
		ft_error("minishell: cd: malloc failed for new PWD");
		return (1);
	}
	if (ft_setenv("PWD", new_pwd, env) != 0)
		status = 1;
	free(new_pwd);
	return (status);
}

int	ft_cd(t_shell *shell)
{
	char	*target_path;
	char	*old_pwd;
	int		status;

	if (check_cd_args(shell->cmds))
		return (1);
	 if (!shell->cmds->args[1])
	{
		target_path = get_env_val(shell->env, "HOME");
		if (!target_path)
		{
			write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
			return (1);
		}
	}
	else
	{
		target_path = ft_strdup(shell->cmds->args[1]);
		if (!target_path)
		{
			ft_error("minishell: cd: malloc failed for target path");
			return (1);
		}
	}
	old_pwd = get_old_pwd();
	if (!old_pwd)
	{
		free(target_path);
		return (1);
	}
	if (try_chdir(target_path))
	{
		free(target_path);
		free(old_pwd);
		return (1);
	}
	status = update_pwd_env(&shell->env, old_pwd); //check the triple pointer in called function
	free(old_pwd);
	free(target_path);
	return (status);
}

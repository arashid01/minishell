/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 13:51:50 by nora              #+#    #+#             */
/*   Updated: 2025/06/12 13:46:31 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_old_pwd(void)
{
	char	buffer[1024];

	if (!getcwd(buffer, sizeof(buffer)))
	{
		perror("minishell: cd: getcwd");
		return (NULL);
	}
	return (ft_strdup(buffer));
}

int	try_chdir(char *target)
{
	if (chdir(target) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

int	update_pwd_env(char ***env, char *old_pwd)
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

int	change_dir_and_update(char ***env, char *target_path)
{
	char	*old_pwd;
	int		status;

	old_pwd = get_old_pwd();
	if (!old_pwd)
	{
		free(target_path);
		return (1);
	}
	if (try_chdir(target_path))
	{
		free(old_pwd);
		free(target_path);
		return (1);
	}
	status = update_pwd_env(env, old_pwd);
	free(old_pwd);
	free(target_path);
	return (status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:52:39 by amal              #+#    #+#             */
/*   Updated: 2025/06/15 11:13:22 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_exp_assign(char *arg, char *equal_sign_pos, char ***env)
{
	char	*name_part;
	char	*value_part;
	int		status;

	status = 0;
	name_part = ft_substr(arg, 0, equal_sign_pos - arg);
	if (!name_part)
	{
		perror("minishell: export: malloc failed");
		return (1);
	}
	value_part = equal_sign_pos + 1;
	if (ft_setenv(name_part, value_part, env) != 0)
		status = 1;
	free(name_part);
	return (status);
}

static int	handle_exp_declare(char *arg, char ***env)
{
	int	status;

	status = 0;
	if (get_env_val(*env, arg) == NULL)
	{
		if (ft_setenv(arg, "", env) != 0)
			status = 1;
	}
	return (status);
}

int	process_exp_arg(char *arg, char ***env)
{
	char	*equal_sign_pos;

	if (!is_valid_name(arg))
	{
		write(STDERR_FILENO, "minishell: export: ", 20);
		write(STDERR_FILENO, arg, ft_strlen(arg));
		write(STDERR_FILENO, ": not a valid identifier\n", 26);
		return (1);
	}
	equal_sign_pos = ft_strchr(arg, '=');
	if (equal_sign_pos != NULL)
		return (handle_exp_assign(arg, equal_sign_pos, env));
	else
		return (handle_exp_declare(arg, env));
}

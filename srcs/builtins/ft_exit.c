/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 09:58:28 by nora              #+#    #+#             */
/*   Updated: 2025/06/05 14:52:00 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

static int  skip_ws_sign(const char *str, int *i, int *sign_val)
{
	*sign_val = 1;
	while (ft_isspace(str[*i]))
		(*i)++;
	if (str[*i] == '-')
	{
		*sign_val = -1;
		(*i)++;
	}
	else if (str[*i] == '+')
	{
		*sign_val = 1;
		(*i)++;
	}
	if (!ft_isdigit(str[*i]))
		return (1);
	return (0);
}

static int  parse_num_ovf(const char *str, int *i, long long *val)
{
	long long	prev;

	*val = 0;
	while (ft_isdigit(str[*i]))
	{
		prev = *val;
		*val = (*val) * 10 + (str[*i] - '0');
		if (((*val) / 10) != prev)
			return (1);
		(*i)++;
	}
	return (0);
}

//check other way to write function
static long long	ft_atolli(const char *str, int *status)
{
	int			i;
	int			sign_val;
	long long	val;

	i = 0;
	if (status)
		*status = 0;
	if (skip_ws_sign(str, &i, &sign_val) != 0)
	{
		if (status)
			*status = 2;
		return (0);
	}
	if (parse_num_ovf(str, &i, &val) != 0)
	{
		if (status)
			*status = 1;
		return (0);
	}
	if (str[i] != '\0')
	{
		if (status)
			*status = 2;
	}
	return (val * sign_val);
}

void	ft_exit(t_shell *shell)
{
	long long	exit_code;
	int			status;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (!shell->cmds || !shell->cmds->args || !shell->cmds->args[1])
	{
		free_cmds(shell->cmds);
		exit(shell->exit_code);
	}
	exit_code = ft_atolli(shell->cmds->args[1], &status);
	shell->exit_code = exit_code;
	if (status != 0)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(shell->cmds->args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		free_cmds(shell->cmds);
		exit(255);
	}
	if (shell->cmds->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		shell->exit_code = 1;
		return ;
	}
	free_cmds(shell->cmds);
	exit((unsigned char)shell->exit_code);
}

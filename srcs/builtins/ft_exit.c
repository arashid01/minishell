/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 09:58:28 by nora              #+#    #+#             */
/*   Updated: 2025/05/27 03:55:32 by amal             ###   ########.fr       */
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

static int  parse_num_ovf(const char *str, int *i, long long *value_accumulated)
{
	long long	prev;

	*value_accumulated = 0;

	while (ft_isdigit(str[*i]))
	{
		prev = *value_accumulated;
		*value_accumulated = (*value_accumulated) * 10 + (str[*i] - '0');
		if (((*value_accumulated) / 10) != prev)
			return (1);
		(*i)++;
	}
	return (0);
}

static long long	ft_atolli(const char *str, int *status)
{
	int			i;
	int			sign_val;
	long long	value_accumulated;

	i = 0;
	if (status)
		*status = 0;
	if (skip_ws_sign(str, &i, &sign_val) != 0)
	{
		if (status)
			*status = 2;
		return (0);
	}
	if (parse_num_ovf(str, &i, &value_accumulated) != 0)
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
	return (value_accumulated * sign_val);
}

void	ft_exit(t_cmd *cmd)
{
	long long	exit_code;
	int			status;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (!cmd || !cmd->args || !cmd->args[1])
		exit(g_exit_status);
	exit_code = ft_atolli(cmd->args[1], &status);
	if (status != 0)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(255);
	}
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		g_exit_status = 1;
		return ;
	}
	exit((unsigned char)exit_code);
}

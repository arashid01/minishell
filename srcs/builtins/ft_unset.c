/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:59:18 by nora              #+#    #+#             */
/*   Updated: 2025/06/15 11:13:42 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_unset_error(char *arg)
{
	ft_putstr_fd("minishell: unset: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": not a valid identifier", STDERR_FILENO);
}

int	ft_unset(t_shell *shell, t_cmd *cmds)
{
	int		i;
	int		status;
	char	**args;

	if (!shell || !cmds || !cmds->args || !cmds->args[1])
		return (0);
	args = cmds->args;
	i = 1;
	status = 0;
	while (args[i])
	{
		if (!is_valid_name(args[i]))
		{
			print_unset_error(args[i]);
			status = 1;
		}
		else
			ft_unsetenv(args[i], &shell->env);
		i++;
	}
	return (status);
}

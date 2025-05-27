/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:50:39 by amal              #+#    #+#             */
/*   Updated: 2025/05/27 19:10:08 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_exp_var(char *env_var_str)
{
	char	*equal_sign_pos;

	write(STDOUT_FILENO, "declare -x ", 11);
	equal_sign_pos = ft_strchr(env_var_str, '=');
	if (equal_sign_pos != NULL)
	{
		write(STDOUT_FILENO, env_var_str, equal_sign_pos - env_var_str);
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, equal_sign_pos + 1, ft_strlen(equal_sign_pos + 1));
		write(STDOUT_FILENO, "\"", 1);
	}
	else
		write(STDOUT_FILENO, env_var_str, ft_strlen(env_var_str));
	write(STDOUT_FILENO, "\n", 1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_positional_params.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 07:10:01 by amal              #+#    #+#             */
/*   Updated: 2025/06/12 13:46:31 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_arg_pos(char *input, int *idx)
{
	int		start_num;
	char	*num_str;

	start_num = *idx;
	while (ft_isdigit(input[*idx]))
		(*idx)++;
	num_str = ft_substr(input, start_num, *idx - start_num);
	return (num_str);
}

char	*get_shell_arg(char *input, int *idx, char **argv)
{
	char	*num_str;
	int		param_idx;
	char	*expanded_val;

	num_str = get_arg_pos(input, idx);
	if (!num_str)
		return (ft_strdup(""));
	param_idx = ft_atoi(num_str);
	free(num_str);
	if (param_idx >= 0 && argv && argv[param_idx])
		expanded_val = ft_strdup(argv[param_idx]);
	else
		expanded_val = ft_strdup("");
	if (!expanded_val)
		return (ft_strdup(""));
	return (expanded_val);
}

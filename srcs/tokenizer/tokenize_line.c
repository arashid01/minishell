/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:41:48 by amal              #+#    #+#             */
/*   Updated: 2025/06/15 11:21:28 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	init_status(t_status *status)
{
	status->normal = 1;
	status->s_quote = 0;
	status->d_quote = 0;
}

static void	get_tokens(t_tkn_data *data)
{
	while (data->line[data->i])
	{
		if (data->status->normal && data->line[data->i] == 32)
		{
			data->i++;
			continue ;
		}
		else if (data->status->normal && is_operator(data->line[data->i]))
		{
			handle_operator(data->line, &data->i, data->token_list);
			continue ;
		}
		else
			handle_word(data);
	}
}

void	tokenize_line(char *line, t_shell *shell)
{
	t_status	status;
	t_tkn_data	data;

	shell->tkn = NULL;
	init_status(&status);
	data.line = line;
	data.i = 0;
	data.status = &status;
	data.token_list = &shell->tkn;
	data.shell = shell;
	get_tokens(&data);
}

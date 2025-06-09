/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 06:48:48 by amal              #+#    #+#             */
/*   Updated: 2025/06/09 12:26:18 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_redirs(t_redir *redir_list)
{
	t_redir	*tmp;

	while (redir_list)
	{
		tmp = redir_list;
		redir_list = redir_list->next;
		free(tmp->outfile);
		free(tmp);
	}
}

void	free_cmds(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list;
		cmd_list = cmd_list->next;
		if (tmp->infile)
			free(tmp->infile);
		if (tmp->delim)
			free(tmp->delim);
		if (tmp->args)
			free_arr(tmp->args);
		if (tmp->outfiles)
			free_redirs(tmp->outfiles);
		if (tmp)
			free(tmp);
	}
}

void	free_tokens(t_token *token_list)
{
	t_token	*tmp;

	while (token_list)
	{
		tmp = token_list;
		token_list = token_list->next;
		free(tmp->val);
		free(tmp);
	}
}

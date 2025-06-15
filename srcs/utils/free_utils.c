/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 06:48:48 by amal              #+#    #+#             */
/*   Updated: 2025/06/15 14:56:50 by amrashid         ###   ########.fr       */
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

void	free_redirs(t_redir *redir)
{
	t_redir *tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->target)
			free(tmp->target);
		if (tmp->content)
			free(tmp->content);
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
		free_redirs(tmp->redirs);
		if (tmp->args)
			free_arr(tmp->args);
		if (tmp)
			free(tmp);
	}
}

void	free_tokens(t_token **token_list)
{
	t_token	*tmp;

	while (*token_list)
	{
		tmp = *token_list;
		*token_list = (*token_list)->next;
		if (tmp->val)
			free(tmp->val);
		free(tmp);
	}
}

void	free_shell(t_shell *shell)
{
	free_arr(shell->env);
	free_arr(shell->argv);
	free(shell);
}

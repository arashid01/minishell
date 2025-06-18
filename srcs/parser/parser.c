/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:21:30 by amal              #+#    #+#             */
/*   Updated: 2025/06/18 15:05:05 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

static void	append_arg(t_cmd *cmd, char *arg)
{
	int		len;
	int		i;
	char	**new_args;

	len = 0;
	while (cmd->args && cmd->args[len])
		len++;
	new_args = malloc(sizeof(char *) * (len + 2));
	if (!new_args)
		return ;
	i = 0;
	while (i < len)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[len] = arg;
	new_args[len + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}

static void	append_redir(t_cmd *cmd, int type, char *target)
{
	t_redir	*redir;
	t_redir	*tmp;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return ;
	redir->type = type + 4;
	redir->target = target;
	redir->content = NULL;
	redir->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
}

static int	is_redir_token(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

void	parse_tokens(t_token *tok, t_shell *shell)
{
	t_cmd	*cmd;
	t_cmd	*cmd_head;
	t_cmd	*last_cmd;

	cmd_head = NULL;
	last_cmd = NULL;
	while (tok)
	{
		cmd = init_cmd();
		while (tok && tok->type != PIPE)
		{
			if (tok->type == WORD)
				append_arg(cmd, ft_strdup(tok->val));
			else if (is_redir_token(tok->type))
			{
				if (!tok->next || tok->next->type != WORD)
				{
					perror("Syntax error near unexpected token");
					free_cmds(cmd_head);
					shell->cmds = NULL;
					return ;
				}
				append_redir(cmd, tok->type, ft_strdup(tok->next->val));
				tok = tok->next;
			}
			tok = tok->next;
		}
		if (!cmd_head)
			cmd_head = cmd;
		else
			last_cmd->next = cmd;
		last_cmd = cmd;
		if (tok && tok->type == PIPE)
			tok = tok->next;
	}
	shell->cmds = cmd_head;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:35:41 by amal              #+#    #+#             */
/*   Updated: 2025/06/06 10:09:11 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_cmd	*init_cmd(t_token **token_list)
{
	t_cmd *cmd;
	
	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = build_argv(token_list);
	cmd->infile = NULL;
	cmd->outfiles = NULL;
	cmd->delim = NULL;
	cmd->next = NULL;
	return (cmd);
}


static void handle_heredoc_token(t_cmd *cmd, t_shell *shell)
{
	if (shell->tkn->next && shell->tkn->next->type == WORD)
	{
		cmd->delim = ft_strdup(shell->tkn->next->val);
		shell->tkn = shell->tkn->next->next;
	}
}


static void handle_io_redirection(t_cmd *cmd, t_shell *shell)
{
	t_token *redir = shell->tkn;
	t_token *word = redir->next;
	t_redir *out_redir, *tmp;

	if (!word || word->type != WORD)
		return;

	if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
	{
		out_redir = malloc(sizeof(t_redir));
		if (!out_redir)
		{
			perror("minishell: malloc failed");
			exit(1);
		}
		out_redir->outfile = ft_strdup(word->val);
		out_redir->append = (redir->type == REDIR_APPEND);
		out_redir->next = NULL;
		if (!cmd->outfiles)
			cmd->outfiles = out_redir;
		else
		{
			tmp = cmd->outfiles;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = out_redir;
		}
	}
	else if (redir->type == REDIR_IN)
	{
		if (cmd->infile)
			free(cmd->infile);
		cmd->infile = ft_strdup(word->val);
	}
	shell->tkn = word->next;
}


static void parse_redirections(t_cmd *cmd, t_shell *shell)
{
	while (shell->tkn)
	{
		if (is_heredoc(shell->tkn))
			handle_heredoc_token(cmd, shell);
		else if (is_redirection(shell->tkn))
			handle_io_redirection(cmd, shell);
		else if (shell->tkn->type == PIPE)
			break;
		else
			break;
	}
}


t_cmd *parse_tokens(t_shell *shell)
{
	t_cmd *cmd;

	if (!shell->tkn)
		return (NULL);
	cmd = init_cmd(&shell->tkn);
	print_cmds(cmd);
	parse_redirections(cmd, shell);
	printf("After parsing redirections:\n");
	print_cmds(cmd);
	if (shell->tkn && shell->tkn->type == PIPE)
	{
		shell->tkn = shell->tkn->next;
		cmd->next = parse_tokens(shell);
	}
	return cmd;
}


void print_cmds(t_cmd *cmd)
{
	int	i;
	
	while (cmd)
	{
		printf("Command:\n");
		for (i = 0; cmd->args && cmd->args[i]; i++)
			printf("  Arg[%d]: %s\n", i, cmd->args[i]);
		printf("  Infile: %s\n", cmd->infile ? cmd->infile : "(none)");
		t_redir *r = cmd->outfiles;
		while (r)
		{
			printf("  Outfile: %s (append: %d)\n", r->outfile, r->append);
			r = r->next;
		}
		printf("  Heredoc delim: %s\n", cmd->delim ? cmd->delim : "(none)");
		printf("\n");
		cmd = cmd->next;
	}
}

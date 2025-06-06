/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:35:41 by amal              #+#    #+#             */
/*   Updated: 2025/06/05 15:08:30 by amrashid         ###   ########.fr       */
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
	return (cmd);
}

static void	handle_heredoc_token(t_shell *shell)
{
	if ((shell->tkn)->next && (shell->tkn)->next->type == WORD)
	{
		shell->cmds->delim = ft_strdup((shell->tkn)->next->val);
		shell->tkn = (shell->tkn)->next->next;
	}
}

static void	handle_io_redirection(t_shell *shell)
{
	t_token	*redir;
	t_token	*word;
	t_redir	*out_redir;
	t_redir	*tmp;

	redir = shell->tkn;
	word = redir->next;
	if (!word || word->type != WORD)
		return ;
	out_redir = malloc(sizeof(t_redir));
	if (!out_redir)
	{
		perror("minishell: malloc failed");
		exit(1);
	}
	out_redir->outfile = ft_strdup(word->val);
	out_redir->append = (redir->type == REDIR_APPEND);
	out_redir->next = NULL;
	if (!shell->cmds->outfiles)
		shell->cmds->outfiles = out_redir;
	else
	{
		tmp = shell->cmds->outfiles;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = out_redir;
	}
	if (redir->type == REDIR_IN)
	{
		free(shell->cmds->infile);
		shell->cmds->infile = ft_strdup(word->val);
	}
	shell->tkn = word->next;
}

static void	parse_redirections(t_shell *shell)
{
	while (shell->tkn)
	{
		if (is_heredoc(shell->tkn))
			handle_heredoc_token(shell);
		else if (is_redirection(shell->tkn))
			handle_io_redirection(shell);
		else if ((shell->tkn)->type == PIPE)
			break ;
		else
			shell->tkn = (shell->tkn)->next;
	}
}

t_cmd	*parse_tokens(t_shell *shell)
{
	if (!shell->tkn)
		return (NULL);
	shell->cmds = init_cmd(&shell->tkn);
	parse_redirections(shell);
	if (has_pipe(&shell->tkn))
		shell->cmds->next = parse_tokens(shell);
	return (shell->cmds);
}

//remove later
void print_cmds(t_cmd *cmd)
{
	int i = 0;
	while (cmd)
	{
		printf("Command:\n");
		while (cmd->args[i])
		{
			printf("  Arg[%d]: %s\n", i, cmd->args[i]);
			i++;
		}
		printf("  Has pipe: %d\n\n", cmd->has_pipe);
		i = 0;
		cmd = cmd->next;
	}
}

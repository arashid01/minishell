/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:35:41 by amal              #+#    #+#             */
/*   Updated: 2025/06/09 12:39:33 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_cmd	*init_cmd(t_token **token_list)
{
	t_cmd	*cmd;

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

static void	parse_redirections(t_cmd *cmd, t_shell *shell)
{
	while (shell->tkn)
	{
		if (is_heredoc(shell->tkn))
			handle_heredoc_token(cmd, shell);
		else if (is_redirection(shell->tkn))
			handle_io_redirection(cmd, shell);
		else if (shell->tkn->type == PIPE)
			break ;
		else
			break ;
	}
}

t_cmd	*parse_tokens(t_shell *shell)
{
	t_cmd	*cmd;

	if (!shell->tkn)
		return (NULL);
	cmd = init_cmd(&shell->tkn);
	parse_redirections(cmd, shell);
	if (shell->tkn && shell->tkn->type == PIPE)
	{
		shell->tkn = shell->tkn->next;
		cmd->next = parse_tokens(shell);
	}
	return (cmd);
}

// void	print_cmds(t_cmd *cmd)
// {
// 	int	i;
	
// 	while (cmd)
// 	{
// 		printf("Command:\n");
// 		for (i = 0; cmd->args && cmd->args[i]; i++)
// 			printf("  Arg[%d]: %s\n", i, cmd->args[i]);
// 		printf("  Infile: %s\n", cmd->infile ? cmd->infile : "(none)");
// 		t_redir *r = cmd->outfiles;
// 		while (r)
// 		{
// 			printf("  Outfile: %s (append: %d)\n", r->outfile, r->append);
// 			r = r->next;
// 		}
// 		printf("  Heredoc delim: %s\n", cmd->delim ? cmd->delim : "(none)");
// 		printf("\n");
// 		cmd = cmd->next;
// 	}
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:31:06 by amal              #+#    #+#             */
/*   Updated: 2025/06/20 15:09:48 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_line(char *line, t_shell *shell)
{
	t_token	*tmp;

	tokenize_line(line, shell);
	free(line);
	if (!shell->tkn)
	{
		printf("minishell: syntax error or tokenization failed\n");
		shell->exit_code = 2;
		return ;
	}
	tmp = shell->tkn;
	parse_tokens(shell->tkn, shell);
	free_tokens(&tmp);
	if (!shell->cmds)
	{
		printf("minishell: parsing failed\n");
		shell->exit_code = 2;
		return ;
	}
	if (handle_heredocs(shell) != 0)
	{
		cleanup_heredoc_files(shell->cmds);
		free_cmds(shell->cmds);
		shell->cmds = NULL;
		shell->exit_code = 1;
		return ;
	}
	execute_command_list(shell);
	cleanup_heredoc_files(shell->cmds);
	free_cmds(shell->cmds);
}

void	init_minishell(t_shell *shell)
{
	char	*line;
	int		i;

	setup_parent_signals();
	while (1)
	{
		if (g_signal_status != 0)
		{
			shell->exit_code = g_signal_status;
			g_signal_status = 0;
		}
		line = readline("minishell$ ");
		if (!line)
		{
			break ;
		}
		if (is_whitespace_line(line) || *line == '\0')// takes whitespaces in history
		{
			free(line);
			continue ;
		}
		add_history(line);
		process_line(line, shell);
	}
	i = shell->exit_code;
	free_shell(shell);
	rl_clear_history();
	exit(i);
}

void	init_shell(t_shell	*shell)
{
	shell->tkn = NULL;
	shell->cmds = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void) argc;
	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		perror("minishell: failed to allocate memory for shell");
		return (1);
	}
	shell->exit_code = 0;
	shell->env = copy_env(envp);
	shell->argv = ft_copy_str_arr(argv);
	if (!shell->env)
	{
		perror("minishell: failed to initialize environment");
		return (1);
	}
	init_shell(shell);
	init_minishell(shell);
	free_arr(shell->env);
	return (0);
}

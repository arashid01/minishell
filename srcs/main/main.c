/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:31:06 by amal              #+#    #+#             */
/*   Updated: 2025/06/21 14:46:59 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_line(char *line, t_shell *shell)
{
	if (!tokenize_and_parse(line, shell))
		return ;
	if (!handle_heredoc_phase(shell))
		return ;
	execute_command_list(shell);
	cleanup_heredoc_files(shell->cmds);
	free_cmds(shell->cmds);
}

static void	main_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		handle_signal_exit_code(shell);
		if (!line)
			break ;
		if (is_line_empty(line))
			continue ;
		add_history(line);
		process_line(line, shell);
	}
}

void	init_minishell(t_shell *shell)
{
	int	code;

	setup_parent_signals();
	main_loop(shell);
	code = shell->exit_code;
	free_shell(shell);
	rl_clear_history();
	exit(code);
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

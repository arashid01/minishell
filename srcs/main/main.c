/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:31:06 by amal              #+#    #+#             */
/*   Updated: 2025/06/06 11:16:30 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_line(char *line, t_shell *shell)
{
	printf("Processing line: %s\n", line);
	shell->tkn = tokenize_line(line, shell);
	printf("Tokenization done\n");
	free(line);
	if (!shell->tkn)
	{
		printf("minishell: syntax error or tokenization failed\n");
		shell->exit_code = 2;
		return ;
	}
	print_tokens(shell->tkn);
	shell->cmds = parse_tokens(shell);
	free_tokens(shell->tkn);
	if (!shell->cmds)
	{
		printf("minishell: parsing failed\n");
		shell->exit_code = 2;
		free_tokens(shell->tkn);
		return ;
	}
	run_cmds(shell);
	free_cmds(shell->cmds);
}

void	init_minishell(t_shell *shell)
{
	char	*line;

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
			break ;
		add_history(line);
		if (is_whitespace_line(line) || *line == '\0')
		{
			free(line);
			continue ;
		}
		process_line(line, shell);
	}
	free_arr(shell->env);
	rl_clear_history();
	exit(shell->exit_code);
}

int main(int argc, char **argv, char **envp)
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
	init_minishell(shell);
	free_arr(shell->env);
	return (shell->exit_code);
}

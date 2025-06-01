/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:31:06 by amal              #+#    #+#             */
/*   Updated: 2025/06/01 10:55:56 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	g_exit_status = 0;

void	process_line(char *line, char ***env)
{
	t_token	*token_list;
	t_cmd	*cmd_list;

	token_list = tokenize_line(line);
	free(line);
	if (!token_list)
	{
		printf("minishell: syntax error or tokenization failed\n");
		g_exit_status = 2;
		return ;
	}
	print_tokens(token_list);
	cmd_list = parse_tokens(token_list);
	// free_tokens(token_list);
	if (!cmd_list)
	{
		printf("minishell: parsing failed\n");
		g_exit_status = 2;
		// free_tokens(token_list);
		return ;
	}
	print_cmds(cmd_list);
	exec_cmd(cmd_list, env, STDIN_FILENO, STDOUT_FILENO);
}

void	handle_input(char ***env, char **argv, char *line)
{
	char	*expanded_line;

	expanded_line = expand_line(*env, line, argv);
	free(line);
	if (!expanded_line)
	{
		printf("minishell: error expanding variables\n");
		g_exit_status = 1;
		return ;
	}
	process_line(expanded_line, env);
}

void	init_minishell(char ***env, char **argv)
{
	char	*line;

	while (1)
	{
		setup_parent_signals();
		line = readline("minishell$ ");
		if (!line)
			break ;
		add_history(line);
		handle_input(env, argv, line);
	}
	rl_clear_history();
	exit(g_exit_status);
}

int main(int argc, char **argv, char **envp)
{
	char	**envp_copy;
	
	(void) argc;
	g_exit_status = 0;
	envp_copy = copy_env(envp);
	if (!envp_copy)
	{
		perror("minishell: failed to initialize environment");
		return (1);
	}
	init_minishell(&envp_copy, argv);
	free_env(envp_copy);
	return (g_exit_status);
}

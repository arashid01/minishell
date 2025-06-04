/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nagha <nagha@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:31:06 by amal              #+#    #+#             */
/*   Updated: 2025/06/04 16:57:34 by nagha            ###   ########.fr       */
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
	cmd_list = parse_tokens(token_list);
	free_tokens(token_list);
	if (!cmd_list)
	{
		printf("minishell: parsing failed\n");
		g_exit_status = 2;
		free_tokens(token_list);
		return ;
	}
	run_cmds(cmd_list, env);
	free_cmds(cmd_list);
}

void	handle_input(char ***env, char **argv, char *line)
{
	char	*expanded_line;

	expanded_line = expand_line(*env, line, argv);
	free(line);
	if (!expanded_line)
	{
		printf("minishell: error expanding variables\n");
		free_arr(*env);
		g_exit_status = 1;
		exit (g_exit_status);
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
		if (is_whitespace_line(line) || *line == '\0')
		{
			free(line);
			continue ;
		}
		handle_input(env, argv, line);
	}
	free_arr(*env);
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
	free_arr(envp_copy);
	return (g_exit_status);
}

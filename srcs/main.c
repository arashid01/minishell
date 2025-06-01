/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:31:06 by amal              #+#    #+#             */
/*   Updated: 2025/05/27 03:43:51 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status = 0;

void	init_minishell(char ***envp_ptr, char **argv)
{
	char	*line;
	t_token	*token_list;
	t_cmd	*cmd_list;
	char	*expanded_line;

	while (1)
	{
		setup_parent_signals();
		line = readline("minishell$ ");
		if (!line)
			break ;
		add_history(line);
		expanded_line = expand_line(*envp_ptr, line, argv);
		free(line);
		if (!expanded_line)
		{
			ft_printf("minishell: error expanding variables\n");
			g_exit_status = 1;
			continue ;
		}
		line = expanded_line;
		token_list = tokenize_line(line);
		free(line);
		if (!token_list)
		{
			ft_printf("minishell: syntax error or tokenization failed\n");
			g_exit_status = 2;
			continue;
		}
		print_tokens(token_list);
		cmd_list = parse_tokens(token_list);
		// free_tokens(token_list);
		if (!cmd_list)
		{
			ft_printf("minishell: parsing failed\n");
			g_exit_status = 2;
			// free_tokens(token_list);
			continue;
		}
		print_cmds(cmd_list);
		exec_cmd(cmd_list, envp_ptr, STDIN_FILENO, STDOUT_FILENO);
	}
	rl_clear_history();
	exit (g_exit_status);
}

int main(int argc, char **argv, char **envp)
{
	char	**envp_copy;
	
	(void) argc;
	g_exit_status = 0;
	envp_copy = copy_env(envp);
	if (!envp_copy)
	{
		perror("minishell : failed to initialize environment");
		return (1);
	}
	init_minishell(&envp_copy, argv);
	free_env(envp_copy);
	return (g_exit_status);
}

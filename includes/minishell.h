/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:03:18 by amrashid          #+#    #+#             */
/*   Updated: 2025/06/15 12:20:33 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <readline/readline.h>
# include <readline/history.h>

# define WORD 1
# define PIPE 2 // |
# define REDIR_IN 3 // <
# define REDIR_OUT 4 // >
# define REDIR_APPEND 5 // >>
# define HEREDOC 6 // <<
# define R_INPUT 7
# define R_OUTPUT 8
# define R_APPEND 9
# define R_HEREDOC 10

typedef struct s_redir
{
	int				type;
	char			*target;
	char			*content; //for heredoc
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_status
{
	int	normal;
	int	s_quote;
	int	d_quote;
}	t_status;

typedef struct s_token
{
	int				type;
	char			*val;
	struct s_token	*next;
}	t_token;

typedef struct s_shell
{
	char	**env;
	char	**argv;
	int		exit_code;
	pid_t	last_pid;
	t_token	*tkn;
	t_cmd	*cmds;
}	t_shell;

typedef struct s_tkn_data
{
	char		*line;
	int			i;
	t_status	*status;
	t_token		**token_list;
	t_shell		*shell;
}	t_tkn_data;

extern int	g_signal_status;

//  ************** tokenization **************
void		tokenize_line(char *line, t_shell *shell);
void		handle_operator(char *line, int *i, t_token **token_list);
void		handle_quotes(char c, t_status *status);
int			is_operator(char c);
char		*save_token(char *start, int len);
void		handle_word(t_tkn_data *data);

//  ************** expansion **************
char		*process_dollar(t_shell *shell, char *input, int *idx);
char		*expand_line(char *line, t_tkn_data *data, int inner);
char		*get_shell_arg(char *input, int *idx, char **argv);
char		*hdl_literal(char *input, int *idx);
char		*exp_squote(char *input, int *idx);
char		*exp_dquote(t_tkn_data *data, char *input, int *idx);
char		*strjoin_and_free(char *s1, char *s2);
char		*exp_alpha_var(char **env, char *input, int *idx);
char		*exp_braced_var(char **env, char *input, int *idx);

//  ************** parsing **************
void		parse_tokens(t_token *token_list, t_shell *shell);
int			handle_heredocs(t_shell *shell);
void		cleanup_heredoc_files(t_cmd *cmd_list);

//  ************** execution **************
void		execute_command_list(t_shell *shell);
int			handle_redirections(t_cmd *cmd);
char		*get_command_path(char *cmd, char **envp);

//  ************** builtins **************
int			is_builtin_cmd(t_cmd *cmd);
int			exec_builtin(t_shell *shell, t_cmd *cmd);
int			ft_cd(t_shell *shell, t_cmd *cmds);
int			ft_echo(t_cmd *cmd);
int			ft_env(t_shell *shell, t_cmd *cmds);
void		ft_exit(t_shell *shell, t_cmd *cmds);
int			ft_export(t_shell *shell, t_cmd *cmds);
int			ft_pwd(t_cmd *cmd);
int			ft_unset(t_shell *shell, t_cmd *cmds);

//  ************** signals **************
void		setup_parent_signals(void);
void		setup_child_signals(void);
void		setup_heredoc_signals(void);

//other utils
char		**copy_env(char **envp);
char		*get_env_val(char **env_arr, const char *var);
int			print_sorted_env(char **sorted_env);
int			check_env_args(const char *name, const char *value, char ***env);
int			find_env_idx(const char *name, char **envp);
void		ft_error(const char *msg);
int			process_exp_arg(char *arg, char ***env);
int			export_display(char **env_arr);
void		print_exp_var(char *env_var_str);
void		free_arr(char **arr);
void		free_redirs(t_redir *redir_list);
void		free_cmds(t_cmd *cmd_list);
void		free_tokens(t_token **token_list);
void		free_shell(t_shell *shell);
int			ft_setenv(const char *name, const char *value, char ***env);
void		ft_sort_str_arr(char **arr);
char		**ft_copy_str_arr(char **arr);
int			is_whitespace_line(char *line);
int			is_valid_char(int c);
int			is_valid_name(const char *name);
int			ft_unsetenv(const char *name, char ***env);
char		*get_old_pwd(void);
int			try_chdir(char *target);
int			update_pwd_env(char ***env, char *old_pwd);
int			change_dir_and_update(char ***env, char *target_path);
int			add_env_var(char ***env, const char *name, const char *value);
char		*create_env_entry(const char *name, const char *value);
long long	ft_atolli(const char *str, int *status);

// void print_cmds(t_cmd *cmd);
// void print_tokens(t_token *token_list);

#endif

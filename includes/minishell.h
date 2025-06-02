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
# define PIPE 2
# define REDIR_IN 3
# define REDIR_OUT 4
# define REDIR_APPEND 5
# define HEREDOC 6

typedef struct s_redir {
	char			*outfile;
	int				append;
	struct s_redir	*next;
} t_redir;

typedef struct s_cmd
{
	int				has_pipe;
	int				has_heredoc;
	char			*infile;
	char			*delim;
	char			**args;
	t_redir			*outfiles;
	struct  s_cmd	*next;
}	t_cmd;

typedef struct s_status
{
	int normal;
	int s_quote;
	int d_quote;
} t_status;

typedef struct s_token
{
	int			 	type;
	char			*val;
	struct s_token  *next;
} t_token;

extern int	g_exit_status;

//  ************** tokenization **************
t_token *tokenize_line(char *line);
int		is_operator(char c);
char	*save_token(char *start, int len);
void	handle_quotes(char c, t_status *status);
void	handle_operator(char *line, int *i, t_token **token_list);
void	handle_word(char *line, int *i, t_status *status, t_token **token_list);

//  ************** expansion **************
char	*expand_dollar_sign(char **env_arr, char *input, int *idx);
char	*expand_line(char **env_arr, char *input_line, char **argv);
char	*hdl_literal(char *input, int *idx);
char	*exp_squote(char *input, int *idx);
char	*exp_dquote(char **env_arr, char *input, int *idx, char **argv);
char	*strjoin_and_free(char *s1, char *s2);
char	*get_env_val(char **env_arr, const char *var);
char	*exp_alpha_var(char **env_arr, char *input, int *idx);
char	*exp_braced_var(char **env_arr, char *input, int *idx);
char	*process_dollar(char **env_arr, char *input, int *idx, char **argv);
char	*get_shell_arg(char *input, int *idx, char **argv);

//  ************** parsing **************
t_cmd	*parse_tokens(t_token *token_list);
char	**build_argv(t_token **token);
int		count_args(t_token *token);
int		has_pipe(t_token **token_list);
int		is_heredoc(t_token *token);
int		is_redirection(t_token *token);

//  ************** execution **************
void	run_cmds(t_cmd *cmd_list, char ***env);
void	handle_heredoc(const char *delim, char **outfile);
pid_t	exec_pipe(t_cmd *cmd_list, char ***env, int in_fd, int out_fd);
void	exec_child(t_cmd *cmd, char **env, int in_fd, int out_fd);
int		exec_builtin_single(t_cmd *cmd, char ***env, int in_fd, int out_fd);
int		handle_in(t_cmd *cmd, int inherited_fd);
int		handle_out(t_cmd *cmd, int inherited_fd);
void	restore_std(int saved_fd, int std_fd);
void	setup_redir(int in_fd, int out_fd);
char	*find_exe(char *cmd, char **envp);

//  ************** builtins **************
int		is_builtin_cmd(t_cmd *cmd);
int		exec_builtin(t_cmd *cmd, char ***env);
int		ft_cd(t_cmd *cmd, char ***env);
int		ft_echo(t_cmd *cmd);
int		ft_env(t_cmd *cmd, char **env_arr);
void	ft_exit(t_cmd *cmd);
int		ft_export(t_cmd *cmd, char ***env);
int		ft_pwd(t_cmd *cmd);
int		ft_unset(t_cmd *cmd, char ***env);

//  ************** utils **************
void	ft_error(const char *msg);
void	ft_sort_str_arr(char **arr);

//  ************** free utils **************
void	free_arr(char **arr);
void	free_redirs(t_redir *redir_list);
void	free_cmds(t_cmd *cmd_list);
void	free_tokens(t_token *token_list);

//  ************** signals **************
void	setup_parent_signals(void);
void	setup_child_signals(void);
void	setup_heredoc_signals(void);

//  remove later (debugging functions)
void	print_tokens(t_token *token);
void	print_cmds(t_cmd *cmd);

//other utils
int		process_exp_arg(char *arg, char ***env);
int		export_display(char **env_arr);
void	print_exp_var(char *env_var_str);
int		is_valid_name(const char *name);
int		is_valid_char(int c);

//  ************** env utils **************
char	**copy_env(char **envp);
char	*get_env_val(char **env_arr, const char *var);
void	free_env(char **envp);
int		print_sorted_env(char **sorted_env);
int		find_env_idx(const char *name, char **envp);
int		check_env_args(const char *name, const char *value, char ***env);
int		ft_unsetenv(const char *name, char ***env);
int		ft_setenv(const char *name, const char *value, char ***env);

#endif
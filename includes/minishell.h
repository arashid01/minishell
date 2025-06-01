#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
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

typedef struct s_cmd
{
	int			 append;
	int			 has_pipe;
	int			 has_heredoc;
	char			*infile;
	char			*outfile;
	char			*delim;
	char			**args;
	struct  s_cmd   *next;
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

//  Declare the global exit status variable
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
t_cmd   *parse_tokens(t_token *token_list);

//  ************** execution **************
void	exec_cmd(t_cmd *cmd, char ***env, int in_fd, int out_fd);
void	hdl_in_redir(t_cmd *cmd, int *in_fd);
void	hdl_out_redir(t_cmd *cmd, int *out_fd);
void	child_process(t_cmd *cmd, char ***env, int in_fd, int out_fd, int *fds);
void	parent_process(t_cmd *cmd, pid_t pid, int in_fd, int *fds, char ***env);
char	*find_exe(char *cmd, char **envp);
int		is_absolute_path(const char *cmd);
void	handle_heredoc(const char *delimiter, char **outfile);

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
void	free_arr(char **arr);
void	ft_sort_string_array(char **arr);

//  ************** signals **************
void	setup_parent_signals(void);
void	setup_child_signals(void);
void	setup_heredoc_signals(void);

//  remove later (debugging functions)
void	print_tokens(t_token *token);
void	print_cmds(t_cmd *cmd);

//other utils

char	**copy_env(char **envp);
void	free_env(char **envp);
int		print_sorted_env(char **sorted_env);
int		process_exp_arg(char *arg, char ***env);
int		exp_display_mode(char **env_arr);
void	print_exp_var(char *env_var_str);
int		is_valid_name(const char *name);
int		is_valid_char(int c);
int		ft_unsetenv(const char *name, char ***env);
int		ft_setenv(const char *name, const char *value, char ***env);


#endif
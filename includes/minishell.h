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
	int			 type;
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
char	*handle_dollar_sign_expansion(char **env_array, char *input, int *idx);
char	*expand_input_line(char **env_array, char *input_line, char **argv);
char	*handle_literal_character(char *input, int *idx);
char	*handle_single_quote_expansion(char *input, int *idx);
char	*handle_double_quote_expansion(char **env_array, char *input, int *idx, char **argv);
char	*strjoin_and_free(char *s1, char *s2);
char	*get_env_value(char **env_array, const char *var);
char	*get_expanded_alpha_var(char **env_array, char *input, int *idx);
char	*get_expanded_braced_var(char **env_array, char *input, int *idx);
char	*handle_dollar_sign_expansion_dispatch(char **env_array, char *input, int *idx, char **argv);
char	*get_expanded_positional_param(char *input, int *idx, char **argv);

//  ************** parsing **************
t_cmd   *parse_tokens(t_token *token_list);

//  ************** execution **************
void	execute_command(t_cmd *cmd, char ***envp_ptr, int in_fd, int out_fd);
void	handle_input_redirection(t_cmd *cmd, int *in_fd);
void	handle_output_redirection(t_cmd *cmd, int *out_fd);
void	child_process(t_cmd *cmd, char ***envp_ptr, int in_fd, int out_fd, int *fds);
void	parent_process(t_cmd *cmd, pid_t pid, int in_fd, int *fds, char ***envp_ptr);
char	*find_exe(char *cmd, char **envp);

//  ************** builtins **************
int		is_builtin_cmd(t_cmd *cmd);
int		execute_builtin(t_cmd *cmd, char ***envp_ptr);
int		ft_cd(t_cmd *cmd, char ***envp_ptr);
int		ft_echo(t_cmd *cmd);
int		ft_env(t_cmd *cmd, char **envp_arr);
void	ft_exit(t_cmd *cmd);
int		ft_export(t_cmd *cmd, char ***envp_ptr);
int		ft_pwd(t_cmd *cmd);
int		ft_unset(t_cmd *cmd, char ***envp_ptr);

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
int		process_exp_arg(char *arg, char ***envp_ptr);
int		exp_display_mode(char **envp_arr);
void	print_exp_var(char *env_var_str);
int		is_valid_name(const char *name);
int		is_valid_char(int c);
int		ft_unsetenv(const char *name, char ***envp_ptr);
int		ft_setenv(const char *name, const char *value, char ***envp_ptr);


#endif
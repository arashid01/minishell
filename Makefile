NAME = minishell
CC = cc

LIBFT_DIR = includes/libft
LIBFT = $(LIBFT_DIR)/libft.a


CFLAGS = -Wall -Werror -Wextra -g -I$(LIBFT_DIR)
LDFLAGS = -L$(LIBFT_DIR) -lft  -lreadline

SRC_PATH = srcs/
OBJ_PATH = objs/

SRC =	main/main.c \
SRC =	main/main_utils.c \
		main/signals.c \
		main/signals2.c \
		utils/error_utils.c \
		utils/free_utils.c \
		utils/str_utils.c \
		utils/str_utils2.c \
		utils/env_utils.c \
		utils/env_utils2.c \
		utils/export_args.c \
		utils/export_display.c \
		utils/export_print.c \
		utils/str_validation.c \
		utils/setenv_utils.c \
		utils/setenv_utils2.c \
		utils/unsetenv_utils.c \
		utils/cd_utils.c \
		utils/exit_utils.c \
		tokenizer/tokenize_line.c \
		tokenizer/tokenize_operators.c \
		tokenizer/tokenize_quotes.c \
		tokenizer/tokenize_utils.c \
		tokenizer/tokenize_words.c \
		expansion/expand_dollar_sign.c \
		expansion/expand_line.c \
		expansion/expand_quotes.c \
		expansion/expand_utils.c \
		expansion/expand_vars_alpha.c \
		expansion/expand_vars_braced.c \
		expansion/expand_positional_params.c \
		builtins/ft_builtin.c \
		builtins/ft_echo.c \
		builtins/ft_cd.c \
		builtins/ft_pwd.c \
		builtins/ft_export.c \
		builtins/ft_env.c \
		builtins/ft_unset.c \
		builtins/ft_exit.c \
		parser/parser.c \
		parser/parser_utils.c \
		heredoc/heredoc.c \
		heredoc/heredoc_loop.c \
		heredoc/heredoc_expansion.c \
		execution/exec.c \
		execution/exec_utils.c \
		execution/exec_utils2.c \
		execution/handle_redir.c

OBJ = $(SRC:.c=.o)

SRCS = $(addprefix $(SRC_PATH), $(SRC))
OBJS = $(patsubst $(SRC_PATH)%.c, $(OBJ_PATH)%.o, $(SRCS))

all: $(LIBFT) $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_PATH)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

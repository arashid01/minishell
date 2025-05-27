NAME = minishell
CC = cc

LIBFT_DIR = includes/libft
LIBFT = $(LIBFT_DIR)/libft.a

READLINE_DIR = /usr/local/Cellar/readline/8.2.13

CFLAGS = -Wall -Werror -Wextra -I$(LIBFT_DIR) -I$(READLINE_DIR)/include
LDFLAGS = -L$(LIBFT_DIR) -lft -L$(READLINE_DIR)/lib -lreadline -lncurses

SRC_PATH = srcs/
OBJ_PATH = objs/

SRC =	main.c \
		signals.c \
		utils/general_utils.c \
		utils/env_utils.c \
		utils/export_args.c \
		utils/export_display.c \
		utils/export_print.c \
		utils/str_validation.c \
		utils/setenv_utils.c \
		utils/unsetenv_utils.c \
		tokenizer/tokenize_line.c \
		tokenizer/tokenize_operators.c \
		tokenizer/tokenize_quotes.c \
		tokenizer/tokenize_utils.c \
		tokenizer/tokenize_words.c \
		parser/parser.c \
		expansion/expand_dollar_sign.c \
		expansion/expand_input_line.c \
		expansion/expand_quotes.c \
		expansion/expand_utils.c \
		expansion/expand_vars_alpha.c \
		expansion/expand_vars_braced.c \
		expansion/expand_positional_params.c \
		execution/execute_commands.c \
		execution/execute_processes.c \
		execution/execute_utils.c \
		builtins/ft_builtin.c \
		builtins/ft_echo.c \
		builtins/ft_cd.c \
		builtins/ft_pwd.c \
		builtins/ft_export.c \
		builtins/ft_env.c \
		builtins/ft_unset.c \
		builtins/ft_exit.c \

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

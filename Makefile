NAME 			= minishell

SRCS_DIR   		= srcs/
SRCS_FILES		= main.c export_utils.c args_checker.c builtins.c execution.c free.c \
				  init.c parsing.c parsing_utils.c prepare_exec.c redirection.c signal.c \
				  utils_1.c utils_2.c utils_3.c utils_builtins.c utils_execution.c \
				  utils_print.c utils_redirection.c var_expansion.c utils_expansion.c error.c signal_utils.c \
				  close_fds.c exit.c split_token.c
SRCS 			= $(addprefix $(SRCS_DIR), $(SRCS_FILES))

CC 				= @gcc
CFLAGS 			= -Werror -Wall -Wextra -g
READLINE 		= -lcurses lib/libreadline.a lib/libhistory.a
OBJS_DIR		= objs/
OBJS 			= $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))

HEADER_DIR      = include/

GREEN 			= \033[1;32m
RESET_COLOR 	= \033[0;00m
BRANCH 			?= $(shell bash -c 'read -p "Branch: " branch; echo $$branch')
COMMIT 			?= $(shell bash -c 'read -p "Commit: " commit; echo $$commit')
PATH_SUBMODULE 	?= $(shell bash -c 'read -p "Adress of the submodule: " path_submodule; echo $$path_submodule')
NAME_SUBMODULE 	?= $(shell bash -c 'read -p "Name of the submodule: " name_submodule; echo $$name_submodule')

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c 
				$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

all: 			objs $(NAME)

ifeq ($(shell arch), i386)
$(NAME): 		$(OBJS)
				@echo "Compiling for MacOS🌪 🔥🌊"
				@$(MAKE) -C ./Libft
				@$(CC) $(CFLAGS) $(OBJS) $(READLINE) ./Libft/libft.a -o $(NAME)
				@echo "$(GREEN)Everything is top shape tiguidou 🐥$(RESET_COLOR)"
endif
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
$(NAME): 		$(OBJS)
				@echo "Compiling for Linux 🌪 🔥🌊"
				@$(MAKE) -C ./Libft
				@$(CC) $(CFLAGS) $(OBJS) -lreadline ./Libft/libft.a -o $(NAME)
				@echo "$(GREEN)Everything is top shape tiguidou 🐥$(RESET_COLOR)"
endif

clean:
				@echo "Cleaning 🌪 🔥🌊"
				@$(MAKE) -C ./Libft fclean
				@rm -rf $(OBJS)
				@rm -rf $(OBJS_DIR)

fclean: 		clean
				@rm -rf $(NAME)

re: 			fclean all

sub:
				git submodule update --init --recursive

objs:
				@mkdir -p objs

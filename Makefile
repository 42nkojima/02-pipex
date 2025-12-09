# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/29 23:29:27 by nkojima           #+#    #+#              #
#    Updated: 2025/12/09 21:59:19 by nkojima          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ===============================
#             Color             #
# ===============================
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
RESET = \033[0m

# ===============================
#            Settings           #
# ===============================
NAME = pipex
CFLAGS = -Wall -Wextra -Werror

# ===============================
#         Directory Paths       #
# ===============================
LIBFT_DIR = libs/libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
OBJS_DIR = objs
INCLUDES_DIR = includes

# ===============================
#         Source Files          #
# ===============================
# Common
SRCS_COMMON = common/path.c common/file.c common/error.c common/utils.c

# Mandatory
SRCS_MANDATORY = mandatory/main.c mandatory/process.c mandatory/pipe.c
SRCS = $(SRCS_MANDATORY) $(SRCS_COMMON)

# Bonus
SRCS_BONUS_ONLY = bonus/main_bonus.c bonus/multi_pipe_bonus.c \
                  bonus/heredoc_bonus.c bonus/process_bonus.c
SRCS_BONUS = $(SRCS_BONUS_ONLY) $(SRCS_COMMON)

OBJ_FILES = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
OBJ_FILES_BONUS = $(addprefix $(OBJS_DIR)/, $(SRCS_BONUS:.c=.o))

# ===============================
#         Build Rules           #
# ===============================
all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJ_FILES)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) $(LIBFT)
	@echo "$(NAME): $(GREEN)object files were created $(RESET)"
	@echo "$(NAME): $(YELLOW)$(NAME)$(RESET) $(GREEN)was created$(RESET)"

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -I$(SRC_DIR) -I$(LIBFT_DIR) -c -o $@ $<
	@echo "$(NAME): $(YELLOW)$@$(RESET) $(GREEN)was created$(RESET)"

bonus: $(LIBFT) $(OBJ_FILES_BONUS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES_BONUS) $(LIBFT)
	@echo "$(NAME): $(GREEN)bonus object files were created $(RESET)"
	@echo "$(NAME): $(YELLOW)$(NAME) [BONUS]$(RESET) $(GREEN)was created$(RESET)"


debug: $(LIBFT) $(OBJ_FILES)
	@$(CC) $(CFLAGS) -fsanitize=address -o $(NAME) $(OBJ_FILES) $(LIBFT)
	@echo "$(NAME): $(GREEN)debug build with address sanitizer was created$(RESET)"

debug_bonus: $(LIBFT) $(OBJ_FILES_BONUS)
	@$(CC) $(CFLAGS) -fsanitize=address -o $(NAME) $(OBJ_FILES_BONUS) $(LIBFT)
	@echo "$(NAME): $(GREEN)bonus debug build with address sanitizer was created$(RESET)"

# ===============================
#         Clean Rules           #
# ===============================
clean:
	@if [ -d $(OBJS_DIR) ]; then \
		$(RM) -r $(OBJS_DIR); \
		echo "$(NAME): $(YELLOW)$(OBJS_DIR)$(RESET) $(RED)was deleted$(RESET)"; \
	fi

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@if [ -f $(NAME) ]; then \
		$(RM) $(NAME); \
		echo "$(NAME): $(YELLOW)$(NAME)$(RESET) $(RED)was deleted$(RESET)"; \
	fi

re: fclean all

re_bonus: fclean bonus

.PHONY: all bonus clean fclean re re_bonus debug debug_bonus

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/29 23:29:27 by nkojima           #+#    #+#              #
#    Updated: 2025/11/30 11:24:10 by nkojima          ###   ########.fr        #
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
SRCS =  main.c

OBJ_FILES = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

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

debug: $(LIBFT) $(OBJ_FILES)
	@$(CC) $(CFLAGS) -fsanitize=address -o $(NAME) $(OBJ_FILES) $(LIBFT)
	@echo "$(NAME): $(GREEN)debug build with address sanitizer was created$(RESET)"

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

.PHONY: all clean fclean re debug

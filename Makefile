# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/29 23:29:27 by nkojima           #+#    #+#              #
#    Updated: 2025/11/29 23:34:30 by nkojima          ###   ########.fr        #
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
LIBS_DIR = libs

SRC_DIR = src
OBJS_DIR = objs

# ===============================
#         Source Files          #
# ===============================
SRCS =  main.c

OBJ_FILES = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

# ===============================
#            Libraries          #
# ===============================
LIBS = ft_strlen.c

LIBS_OBJ_FILES = $(addprefix $(OBJS_DIR)/$(LIBS_DIR)/, $(LIBS:.c=.o))

# ===============================
#         Build Rules           #
# ===============================
all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBS_OBJ_FILES)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) $(LIBS_OBJ_FILES)
	@echo "$(NAME): $(GREEN)object files were created $(RESET)"
	@echo "$(NAME): $(YELLOW)$(NAME)$(RESET) $(GREEN)was created$(RESET)"

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(SRC_DIR) -I$(LIBS_DIR) -c -o $@ $<
	@echo "$(NAME): $(YELLOW)$@$(RESET) $(GREEN)was created$(RESET)"

$(OBJS_DIR)/$(LIBS_DIR)/%.o: $(LIBS_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(LIBS_DIR) -c -o $@ $<
	@echo "$(NAME): $(YELLOW)$@$(RESET) $(GREEN)was created$(RESET)"

# ===============================
#         Clean Rules           #
# ===============================
clean:
	@if [ -d $(OBJS_DIR) ]; then \
		rm -rf $(OBJS_DIR); \
		echo "$(NAME): $(YELLOW)$(OBJS_DIR)$(RESET) $(RED)was deleted$(RESET)"; \
	fi

fclean: clean
	@if [ -f $(NAME) ]; then \
		rm -f $(NAME); \
		echo "$(NAME): $(YELLOW)$(NAME)$(RESET) $(RED)was deleted$(RESET)"; \
	fi

re: fclean all

.PHONY: all clean fclean re

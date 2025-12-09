# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/26 16:47:58 by nkojima           #+#    #+#              #
#    Updated: 2025/11/30 11:24:06 by nkojima          ###   ########.fr        #
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
NAME      = libft
CFLAGS    = -Wall -Wextra -Werror
ARFLAGS   = rcs

# ===============================
#         Source & Objects      #
# ===============================
SRC_FILES = isalpha \
			isdigit \
			isalnum \
			isascii \
			isprint \
			strlen \
			memset \
			bzero \
			memcpy \
			memmove \
			strlcpy \
			strlcat \
			toupper \
			tolower \
			strchr \
			strrchr \
			strcmp \
			strncmp \
			memchr \
			memcmp \
			strnstr \
			atoi \
			atold \
			calloc \
			strdup \
			substr \
			strjoin \
			strtrim \
			split \
			itoa \
			strmapi \
			striteri \
			putchar_fd \
			putstr_fd \
			putendl_fd \
			putnbr_fd
SRCS 	   = $(addprefix ft_, $(addsuffix .c, $(SRC_FILES))) get_next_line.c
OBJS       = $(SRCS:.c=.o)

# ===============================
#         Build Rules           #
# ===============================
all: $(NAME).a

$(NAME).a: $(OBJS)
	@$(AR) $(ARFLAGS) $@ $?
	@echo "$(NAME): $(GREEN)object files were created$(RESET)"
	@echo "$(NAME): $(YELLOW)$(NAME).a$(RESET) $(GREEN)was created$(RESET)"

%.o: %.c libft.h
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(NAME): $(YELLOW)$@$(RESET) $(GREEN)was created$(RESET)"

# ===============================
#         Clean Rules           #
# ===============================
clean:
	@$(RM) $(OBJS)
	@echo "$(NAME): $(YELLOW)object files$(RESET) $(RED)were deleted$(RESET)"

fclean: clean
	@$(RM) $(NAME).a
	@echo "$(NAME): $(YELLOW)$(NAME).a$(RESET) $(RED)was deleted$(RESET)"

re: fclean all

.PHONY: all clean fclean re

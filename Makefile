NAME = pipex
BONUS_NAME = pipex_bonus

CC = gcc
CFLAGS = -Werror -Wall -Wextra

RM = rm -rf

SRCS = src/pipex.c src/utils.c
SRCS_BONUS = src_bonus/pipex_bonus.c src/utils.c

OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

LIBFT = libft/libft.a

# Regla para compilar la biblioteca libft
$(LIBFT):
	make -C libft

# Regla para compilar el programa principal
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

# Regla para compilar el programa bonus
$(BONUS_NAME): $(OBJS_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT) -o $(BONUS_NAME)

# Objetivo por defecto
all: $(NAME)

# Limpiar archivos objeto
clean:
	$(RM) $(OBJS) $(OBJS_BONUS)
	make clean -C libft

# Limpiar todo
fclean: clean
	$(RM) $(NAME) $(BONUS_NAME)
	make fclean -C libft

# Reconstruir todo
re: fclean all

# Compilar el bonus
bonus: $(BONUS_NAME)

.PHONY: all clean fclean re bonus
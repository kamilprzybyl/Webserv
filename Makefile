CC		=	c++
CFLAGS	=	-Wall -Wextra -Werror -std=c++98
NAME	=	webserv
SRC		=	server.cpp
OBJ		=	$(SRC:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -fr $(OBJ)

fclean:
	make clean
	rm -fr $(NAME)

re: fclean all
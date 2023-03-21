NAME=ilexer

SRC=ilexer.c
OBJ=$(SRC:.c=.o)

all: $(NAME)


$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)


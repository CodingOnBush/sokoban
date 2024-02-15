CXX = gcc
CXXFLAGS = -Wall -Werror -Wextra 

NCURSELIB = -lncurses

SRC = 	main.c \
		sokoban.c
OBJ = $(SRC:.c=.o)
EXEC = exe

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(NCURSELIB)

re: fclean all

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(EXEC)

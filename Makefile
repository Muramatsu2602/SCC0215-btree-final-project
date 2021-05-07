/**
 * @file Makefile
 * @author 11796444 - Giovanni Shibaki Camargo 
 * @author 11796451 - Pedro Kenzo Muramatsu Carmo
 * @version 0.1
 * @date 2021-05-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */

UTILS=utils/register.c
REGISTRY= registry/linha.c registry/veiculo.c
BINARY_FILE =
CSV = linha.csv veiculo.csv
MAIN= main.c
BINARY= main

FLAGS = -Werror 
FLAGS2 = -Werror -Wall -Wextra -Wpedantic -Wpointer-arith -O3 -march=native
VFLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes

all:
	@gcc $(FLAGS) $(MAIN) $(UTILS) $(REGISTRY) -o $(BINARY) -lm -I Util/

run:
	./$(BINARY)

valval: all
	@echo "Looking for leaks!"
	@valgrind $(VFLAGS) ./$(BINARY)

clean:
	@echo "Cleaning the mess!"
	@rm $(BINARY)
	@rm *.zip

zip:
	@echo "Creating a zip folder with all important files!"
	@zip -r t1-giovanni-11796451.zip utils/*  $(MAIN) Makefile
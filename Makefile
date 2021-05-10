
# @file Makefile
# @author 11796444 - Giovanni Shibaki Camargo 
# @author 11796451 - Pedro Kenzo Muramatsu Carmo
# @version 0.1
# @date 2021-05-07
# 
# @copyright Copyright (c) 2021

UTILS= utils/fileManager.c
REGISTRY= registry/linha.c registry/veiculo.c
MAIN= main.c
BINARY= main

FLAGS = -Werror 
FLAGS2 = -Werror -Wall -Wextra -Wpedantic -Wpointer-arith -O3 -march=native
VFLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes

all:
	@gcc $(FLAGS) $(MAIN) $(UTILS) $(REGISTRY) -o $(BINARY) -lm -I utils/ registry/

run:
	./$(BINARY)

valval: all
	@echo "Looking for leaks!"
	@valgrind $(VFLAGS) ./$(BINARY)

deb_hex: all
	@echo "Hexdumping it all!"
	@./$(BINARY) < 1.in &> 1.out
	@ hexdump -vC 1.out > 1.hexdump
	@ code 1.hexdump

clean:
	@echo "Cleaning the mess!"
	@rm $(BINARY)
	@rm *.zip

zip:
	@echo "Creating a zip folder with all the important files!"
	@zip -r trabalho1-11796444_giovanni-1196451_pedro.zip utils/* registry/* csv/* binary/* $(MAIN) Makefile README.md
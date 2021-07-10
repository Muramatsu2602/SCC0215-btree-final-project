
# @file Makefile
# @author 11796444 - Giovanni Shibaki Camargo 
# @author 11796451 - Pedro Kenzo Muramatsu Carmo
# @version 0.1
# @date 2021-05-07
# 
# @copyright Copyright (c) 2021

UTILS= utils/fileManager.c
REGISTRY= registry/linha.c registry/veiculo.c
INDEX = index/index.c
MAIN= main.c
BINARY= main

FLAGS = -Werror 
FLAGS2 = -Werror -Wall -Wextra -Wpedantic -Wpointer-arith -O3 -march=native
VFLAGS= --leak-check=full --show-leak-kinds=all --track-origins=yes

all:
	@gcc $(FLAGS) $(MAIN) $(UTILS) $(REGISTRY) $(INDEX) -o $(BINARY) -lm -I utils/ -I registry/ -I index/

run:
	./$(BINARY)

test1:
	./$(BINARY) < test/1.in

valval: all
	@echo "Looking for leaks!"
	@valgrind $(VFLAGS) ./$(BINARY)

deb_hex: all
	@echo "Hexdumping it all!"
	@./$(BINARY) < test/0.in &> test/0.out
	@ hexdump -vC bin/veiculo.bin > 0.hexdump
	@ code 0.hexdump

clean:
	@echo "Cleaning the mess!"
	@rm $(BINARY)
	@rm *.zip

zip:
	@echo "Creating a zip folder with all the important files!"
	@zip -r trabalho2-11796444_giovanni-11796451_pedro.zip utils/* registry/* index/* $(MAIN) Makefile README.md
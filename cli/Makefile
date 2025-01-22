.PHONY: all clean
CC = gcc
#SRC = $(wildcard *.c)
SRC = cli.c wallet.c var.c send.c mine.c info.c
FLAGS = -Wall -Werror -Wextra -pedantic -g -pthread
BCHAIN = -lhblk_blockchain -L../blockchain/v0.3 -I../blockchain/v0.3
CRY = -lhblk_crypto -L../crypto -I../crypto
LINKER = -lcrypto -lllist -lssl

NAME = cli
BGEN = ../blockchain/v0.3/provided/_genesis.c



#default
all:
	$(CC) $(SRC) $(BGEN) $(FLAGS) $(BCHAIN) $(CRY) $(LINKER) -o $(NAME)


#clean
clean:
	rm -f $(NAME)

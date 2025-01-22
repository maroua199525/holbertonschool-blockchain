.PHONY: all clean
CC = gcc
CCFLAGS = -c -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -lllist -lssl -lcrypto -pthread -Wno-deprecated-declarations -g -I../../crypto -L../../crypto -lhblk_crypto
AR = ar
ARFLAGS = rcs
NAME = libhblk_blockchain.a
SRC = $(wildcard *.c) $(wildcard transaction/*.c)
OBJ = $(SRC:.c=.o)


#default
all: $(NAME)


#library creation target
$(NAME): $(OBJ)
	$(AR) $(ARFLAGS) $(NAME) $(OBJ)
	rm -f $(OBJ)

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

#clean
clean:
	rm-f $(OBJ) $(NAME)

.PHONY: all clean
CC = gcc
CCFLAGS = -c -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -I. -lssl -lcrypto
AR = ar
ARFLAGS = rcs
NAME = libhblk_crypto.a
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)


# default target
all: $(NAME)

# library creation target
$(NAME): $(OBJ)
	$(AR) $(ARFLAGS) $(NAME) $(OBJ)
	rm -f $(OBJ)

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

# clean
clean:
	rm -f $(OBJ) $(NAME)

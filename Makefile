CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wconversion -std=c99 -pedantic -O2\
	-march=native -mtune=native

main : main.c

.PHONY : clean
clean :
	-rm main 2>/dev/null || true

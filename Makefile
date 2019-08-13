CFLAGS = -Wall -O2
LFLAGS = -lncurses
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

%.o: %.c
	clang $(CFLAGS) -c $< -o $@

blackjack: $(OBJS)
	clang $(CFLAGS) $(OBJS) -o blackjack $(LFLAGS)

clean:
	rm *.o

# name of the program (Minix service)
PROG=lab3

# source code files to be compiled
SRCS = lab3.c kbc.c keyboard.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
# CFLAGS += -pedantic
CPPFLAGS += -pedantic -D LAB3

# list of library dependencies (for Lab 3, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf
LDADD += /home/lcom/labs/lab2/timer.o
LDADD += /home/lcom/labs/lab2/utils.o

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
# name of the program (Minix service)
PROG=lab4

# source code files to be compiled
SRCS = lab4.c mouse.c kbc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
# CFLAGS += -pedantic
CPPFLAGS += -pedantic -D LAB4

# list of library dependencies (for Lab 5, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf
LDADD += /home/lcom/labs/lab2/timer.o
LDADD += /home/lcom/labs/lab2/utils.o
LDADD += /home/lcom/labs/lab3/kbc.o
LDADD += /home/lcom/labs/lab3/keyboard.o
LDADD += /home/lcom/labs/lab4/kbc.o
LDADD += /home/lcom/labs/lab4/mouse.o
# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
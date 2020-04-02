#############################################################
# Makefile for static library.
# make ikcc
#############################################################
#set your own environment option
CC = gcc
C_FLAG += -D_NOMNG -D_FILELINE 
	
EX_FLAG += -lrt

C_FLAG += -g -c  -O2 -Werror 

CFLAGS += $(C_FLAG)

#static library use 'ar' command 
AR = ar

#set your inc and lib
INC = -I../ 

PRG = ./fun 
SRC=$(wildcard  ./*.c ../*.c)
#SRC=$(wildcard ../src/*.c ../src/tool/*.c ../src/jsp_trigger/*.c)
OBJ=$(patsubst %.c,%.o,$(notdir $(SRC)))


all:$(PRG)
$(PRG):$(OBJ)
	$(CC) $(OBJ) $(EX_FLAG) $(LIB) -o $(PRG) 
$(OBJ):$(SRC)
	$(CC) $(CFLAGS) $(SRC) $(EX_FLAG) $(INC)

	
.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	rm -f $(OBJ) $(PRG)

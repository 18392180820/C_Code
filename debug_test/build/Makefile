#############################################################
# Makefile for static library.
# make c_test
#############################################################
#set your own environment option
CC = gcc
C_FLAG += -D_NOMNG -D_FILELINE 

ifeq ($(system), linux)
	LIB += -L../lib/ubuntux64/
	
	EX_FLAG += -lgatewaysdk -lXlinkV4Sdk -lsqlite 
	EX_FLAG += -lpthread -lcjson -llua -lm -ldl \
			-Wl,-E	#lua
	
	X86_DEBUG = 1
else 
	EX_FLAG += -lpthread -Wl,-E
endif

 



#JSP_EX_FLAG += -lubox -ljson_script -lblobmsg_json -ljson-c
C_FLAG += -g -c  -O2 #-Werror
#C_FLAG += -DLUA_SCRIPT
CFLAGS += $(C_FLAG)

#static library use 'ar' command 
AR = ar

#set your inc and lib
INC = -I ../include
#	-I /usr/include/lua5.1
#LIB = -lpthread -L./ -lsvrtool

#make target lib and relevant obj 
PRG = ../test
SRC = $(wildcard ../src/*.c)
#SRC = $(wildcard ../src/*.c ../src/tool/*.c ../src/jsp_trigger/*.c)
OBJ = $(patsubst %.c,%.o,$(notdir $(SRC)))


all:$(PRG)
$(PRG):$(OBJ)
	$(CC) $(OBJ) $(EX_FLAG) $(LIB) -o $(PRG) 
$(OBJ):$(SRC)
	$(CC) $(CFLAGS) $(SRC) $(EX_FLAG) $(INC) 

	
.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	rm -f $(OBJ) $(PRG)

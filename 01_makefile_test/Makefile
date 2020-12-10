### makefile规则：
### target ... : prerequisites ... 
### 	command 


OBJ = main.o log.o

test:$(OBJ)
	cc -o test $(OBJ)
	
$(OBJ):log.h

.PHONY:clean
clean:
	rm test $(OBJ)
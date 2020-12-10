###	targets : prerequisites 
###		command 
### 	... 
###或是这样： 
###	targets : prerequisites ; command 
###		command 
### 	... 


OBJ = main.o log.o

test:$(OBJ)
	cc -o test $(OBJ)
	
$(OBJ):log.h

.PHONY:clean
clean:
	- rm test *.o *.out 
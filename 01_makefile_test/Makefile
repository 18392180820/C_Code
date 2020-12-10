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

.PHONY: clean cleanobj cleanout
clean: cleanobj cleanout
	rm test

cleanobj:
	rm *.o

cleanout:
	rm *.out
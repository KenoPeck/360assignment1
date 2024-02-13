prog: lab1_Peck.o 
	gcc -g lab1_Peck.o -o lab1

lab1_Peck.o: lab1_Peck.c
	gcc -g -Wall -c lab1_Peck.c

clean: 
	-rm *.o 

run:	
	@./lab1
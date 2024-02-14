prog: lab1_Peck.o commands.o
	gcc -g lab1_Peck.o commands.o -o lab1

lab1_Peck.o: lab1_Peck.c
	gcc -g -Wall -c lab1_Peck.c

commands.o: commands.c commands.h
	gcc -g -Wall -c commands.c

clean: 
	-rm *.o 

run:	
	@./lab1

update:
	make prog
	make run
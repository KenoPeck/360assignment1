prog: lab1_Peck.o commands.o
	gcc -g -Wall lab1_Peck.o commands.o -o lab1

lab1_Peck.o: lab1_Peck.c commands.h
	gcc -g -c lab1_Peck.c

commands.o: commands.c commands.h
	gcc -g -c -Wall commands.c

clean: 
	-rm *.o 

run:	
	@./lab1
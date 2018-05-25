all:	learn.c
	gcc -o learn learn.c -Wall -Werror -fsanitize=address
	
clean:	
	rm -f learn

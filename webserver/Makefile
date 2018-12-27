server: ./objects/server.o ./objects/http.o
	gcc -o server ./objects/server.o ./objects/http.o -Wall

./objects/server.o: ./src/server.c ./include/http.h
	gcc -o ./objects/server.o ./src/server.c -c -ansi -pedantic -Wall

./objects/http.o: ./src/http.c ./include/http.h
	gcc -o ./objects/http.o ./src/http.c -c -ansi -pedantic -Wall

clean:
	@rm -f ./objects/*.o server
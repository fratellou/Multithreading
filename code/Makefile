CFLAGS=-Wall -Wextra -Werror
EXIT=$(STRUCT)objects/

all:students threading banker

threading: threading.o
	g++ $(CFLAGS) $(EXIT)threading.o -o threading

students: students.o
	g++ $(CFLAGS) $(EXIT)students.o -o students

banker: banker.o 
	g++ $(CFLAGS) $(EXIT)banker.o -o banker

students.o:
	g++ $(CFLAGS) -c students.cpp -o $(EXIT)students.o

threading.o:
	g++ $(CFLAGS) -c threading.cpp -o $(EXIT)threading.o

banker.o:
	g++ $(CFLAGS) -c banker.cpp -o $(EXIT)banker.o

clean: 
	rm -rf $(EXIT)*.o threading students banker

rebuild:clean all
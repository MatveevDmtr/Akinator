# Переменная CC указывает компилятор, используемый для сборки
CC=g++
#В переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS=-c -DLOGGING
OPTFLAG= -O0

all: build execute clear

build: Akinator_main.o GetPoison.o logging.o
	$(CC) Akinator_main.o GetPoison.o logging.o -o Akinator

Akinator_main.o: src/Akinator_main.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) src/Akinator_main.cpp

GetPoison.o: src/GetPoison/GetPoison.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) src/GetPoison/GetPoison.cpp

logging.o: src/logging/logging.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) src/logging/logging.cpp

clear:
	rm *.o

execute:
	./Akinator

use_valgrind:
	valgrind --leak-check=full ./Akinator
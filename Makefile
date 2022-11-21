# Переменная CC указывает компилятор, используемый для сборки
CC=g++
#В переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS=-c

all: Akinator.exe execute clean	

Akinator.exe: Akinator_main.o GetPoison.o logging.o
	$(CC) Akinator_main.o GetPoison.o logging.o -o Akinator.exe

Akinator_main.o: Akinator_main.cpp
	$(CC) $(CFLAGS) Akinator_main.cpp

GetPoison.o: ..\Processor_v4\GetPoison.cpp
	$(CC) $(CFLAGS) ..\Processor_v4\GetPoison.cpp
#$(CC) $(CFLAGS) D:\Programming\C\Ded_course_1_sem\Processor_v4\GetPoison.cpp

logging.o: ..\Processor_v4\logging.cpp
	$(CC) $(CFLAGS) ..\Processor_v4\logging.cpp

clean:
	rm *.o

execute:
	Akinator.exe
del_exe:
	rm Akinator.exe
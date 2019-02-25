# makefile
# Griffith Droid Racer 2019

COMPILER = g++
CFLAGS = -g -Wall -Wextra -Werror
OPTIMISATION = -O2

all: clean main
clean:
	rm *.o -f 2> /dev/null
	rm *.out -f 2> /dev/null
	rm *.stackdump -f 2> /dev/null
	rm *.gch -f 2> /dev/null
ArduinoController:
	$(COMPILER) $(CFLAGS) $(OPTIMISATION) ArduinoController.cpp -c -o ArduinoController.o
main: ArduinoController
	$(COMPILER) $(CFLAGS) $(OPTIMISATION) ArduinoController.o main.cpp -o main.o

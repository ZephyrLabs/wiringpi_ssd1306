default:
	gcc -c -o ssd1306.o ssd1306.c -lwiringPi -lpthread -lrt -lm -lcrypt
	gcc -c -o main.o main.c -lwiringPi -lpthread -lrt -lm -lcrypt
	gcc -o main main.o ssd1306.o -lwiringPi -lpthread -lrt -lm -lcrypt

clean:
	rm -rf *.o
	rm -rf main
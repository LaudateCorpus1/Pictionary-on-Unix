CC = gcc

CFLAGS = -W -Wall

TARGET = paint



$(TARGET) : main.o event.o drawing.o ipc.o typo.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o event.o drawing.o ipc.o typo.o -lX11 -lpthread
	rm *.o
	./$(TARGET)

typo.o: typo.c
	$(CC) $(CFLAGS) -c -o typo.o typo.c -lX11 -pthread

drawing.o: event.c
	$(CC) $(CFLAGS) -c -o drawing.o drawing.c -lX11 -lpthread

event.o: event.c
	$(CC) $(CFLAGS) -c -o event.o event.c -lX11 -lpthread

ipc.o: ipc.c
	$(CC) $(CFLAGS) -c -o ipc.o ipc.c -lX11 -lpthread

main.o : main.c
	$(CC) $(CFLAGS) -c -o main.o main.c -lX11 -lpthread

clean :
	rm *.o $(TARGET)

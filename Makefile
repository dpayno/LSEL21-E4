CC = gcc
CFLAGS = -Wall


OBJS = main.o fsm.o fsm_led_alarm.o fsm_door_checking.o
HDRS = fsm.h fsm_led_alarm.h fsm_door_checking.h

# Executable
EXE = main

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)

$(OBJS): $(HDRS)


wiringPi.o: wiringPi.c
	$(CC) $(CFLAGS) -c wiringPi.c

fsm.o: fsm.c
	$(CC) $(CFLAGS) -c fsm.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

fsm_led_alarm.o: fsm_led_alarm.c
	$(CC) $(CFLAGS) -c fsm_led_alarm.c

fsm_door_checking.o: fsm_door_checking.c
	$(CC) $(CFLAGS) -c fsm_door_checking.c

clean:
	rm -f $(OBJS) $(EXE) *.bak *~

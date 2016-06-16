CC = ${CROSS_COMPILE}gcc
#LDFLAGS := -lxdma
LDFLAGS := 
CFLAGS := -c -Wall
INCLUDES := -I. -I../dev

EXECUTABLE = \
	demo \
	test
#	app \


.PHONY : all
all: $(EXECUTABLE)


test : xdma-test.o libxdma.o
	$(CC) $< -o $@ $(LDFLAGS)


demo : xdma-demo.o libxdma.o
	$(CC) $< -o $@ $(LDFLAGS)


app : xdma-app.o libxdma.o
	$(CC) $< -o $@ $(LDFLAGS)


libxdma.o :
	$(CC) $(CFLAGS) $(INCLUDES) -fpic libxdma.c


.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@


.PHONY : clean
clean:
	rm -f *.o $(EXECUTABLE)

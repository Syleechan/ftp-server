SOURCES:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,${SOURCES})
client:${OBJS}
	gcc $^ -o $@ -lcrypt

clean:
	rm client ${OBJS}

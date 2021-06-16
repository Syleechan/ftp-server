SOURCES:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,${SOURCES})
main:${OBJS}
	gcc $^ -o $@ -pthread -lcrypt -lmysqlclient

clean:
	rm main ${OBJS}

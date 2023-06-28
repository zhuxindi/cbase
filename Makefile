CBASE_OBJ = stream.o buffer.o event.o rbtree.o pool.o log.o systime.o xstrings.o ring.o
CBASE_TEST = utils_test log_test event_test buffer_test rbtree_test stream_test pool_test xstrings_test ring_test
CBASE_STATIC = libcbase.a
CBASE_DYNAMIC = libcbase.so

CFLAGS = -std=gnu99 -D_GNU_SOURCE -fPIC -pipe -O2 -W -Wall -Wpointer-arith -Wno-unused-parameter -g -I.

all: $(CBASE_STATIC) $(CBASE_DYNAMIC) test

$(CBASE_STATIC): $(CBASE_OBJ)
	ar rcs $@ $^

$(CBASE_DYNAMIC): $(CBASE_OBJ)
	gcc -o $@ -shared $(CFLAGS) $^

test: $(CBASE_TEST)

utils_test: utils_test.c
	gcc -o $@ $(CFLAGS) $^

log_test: log_test.c log.o systime.o
	gcc -o $@ $(CFLAGS) $^

event_test: event_test.c event.o log.o rbtree.o systime.o
	gcc -o $@ $(CFLAGS) $^

buffer_test: buffer_test.c buffer.o log.o pool.o systime.o
	gcc -o $@ $(CFLAGS) $^

rbtree_test: rbtree_test.c rbtree.o log.o systime.o
	gcc -o $@ $(CFLAGS) $^

stream_test: stream_test.c stream.o buffer.o event.o log.o rbtree.o pool.o systime.o
	gcc -o $@ $(CFLAGS) $^

pool_test: pool_test.c pool.o log.o systime.o
	gcc -o $@ $(CFLAGS) $^

xstrings_test: xstrings_test.c xstrings.o
	gcc -o $@ $(CFLAGS) $^

ring_test: ring_test.c ring.o log.o systime.o
	gcc -o $@ $(CFLAGS) $^

stream.o: stream.c stream.h
	gcc -o $@ -c $(CFLAGS) $<

buffer.o: buffer.c buffer.h
	gcc -o $@ -c $(CFLAGS) $<

event.o: event.c event.h
	gcc -o $@ -c $(CFLAGS) $<

rbtree.o: rbtree.c rbtree.h
	gcc -o $@ -c $(CFLAGS) $<

pool.o: pool.c pool.h
	gcc -o $@ -c $(CFLAGS) $<

log.o: log.c log.h
	gcc -o $@ -c $(CFLAGS) $<

systime.o: systime.c systime.h
	gcc -o $@ -c $(CFLAGS) $<

xstrings.o: xstrings.c xstrings.h
	gcc -o $@ -c $(CFLAGS) $<

ring.o: ring.c ring.h
	gcc -o $@ -c $(CFLAGS) $<

clean:
	rm -f $(CBASE_STATIC) $(CBASE_DYNAMIC) $(CBASE_OBJ) $(CBASE_TEST)

/**
 * File: ring.h
 * Author: ZhuXindi
 * Date: 2017-12-22
 */

#ifndef _RING_H
#define _RING_H

#include <stdlib.h>

struct ring {
	char *begin;	/* buffer begin */
	char *end;	/* buffer end */
	char *read;	/* next read position */
	char *write;	/* next write position */
};

#define RING_INITIALIZER(b, s) {	\
	.begin = b,			\
	.end = b + s,			\
	.read = b,			\
	.write = b,			\
}

#define DEFINE_RING(name, s)		\
	char ringbuf##name[s];		\
	struct ring name = RING_INITIALIZER(ringbuf##name, s)

/* initialize a ring that use buf and can contain at most size bytes */
static inline void ring_init(struct ring *ring, char *buf, size_t size)
{
	ring->read = ring->write = ring->begin = buf;
	ring->end = buf + size;
}

/* create a ring that can contain at most size bytes */
struct ring *ring_create(size_t size);

/* destroy a ring */
void ring_destroy(struct ring *ring);

/* push data into ring at next write position */
size_t ring_push(struct ring *ring, const char *ptr, size_t len);

/* pop data from ring at next read position and erased after copy */
size_t ring_pop(struct ring *ring, char *ptr, size_t len);

/* get the size of the ring */
static inline size_t ring_size(const struct ring *ring)
{
	return ring->end - ring->begin;
}

/* get the used size of the ring */
static inline size_t ring_used(const struct ring *ring)
{
	if (ring->read < ring->write)
		return ring->write - ring->read;
	else if (ring->read > ring->write)
		return ring_size(ring) - (ring->read - ring->write);
	else {
		if (ring->read == ring->begin)
			return 0;
		else
			return ring_size(ring);
	}
}

/* get the usable size of the ring */
static inline size_t ring_usable(const struct ring *ring)
{
	return ring_size(ring) - ring_used(ring);
}

#endif /* _RING_H */

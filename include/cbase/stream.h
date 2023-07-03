/**
 * File: stream.h
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#ifndef _STREAM_H
#define _STREAM_H

#include <cbase/event.h>
#include <cbase/buffer.h>

#define STREAM_T_RD	0x00000001
#define STREAM_T_WR	0x00000002
#define STREAM_T_RDWR	(STREAM_T_RD|STREAM_T_WR)

struct stream {
	int fd;			/* attached fd */
	int type;		/* STREAM_T_XXX */
	unsigned int error:1,	/* stream has errors */
		     readable:1,/* stream is readable*/
		     writable:1,/* stream is writable*/
		     eof:1;	/* stream is closed */
	size_t rdbuf_size;	/* read buffer size */
	struct event rev, wev;	/* read and write event */
	struct list_head read_queue, write_queue; /* read and write queue */
	struct pool *pool;
};

/* init a stream */
void stream_init(struct stream *stream, int type, size_t rdbuf_size,
		 struct pool *pool);

/* attach an fd to a stream */
int stream_attach_fd(struct stream *stream, int fd);

/* detach a stream */
void stream_detach(struct stream *stream);

/* write buffer to a stream */
int stream_write(struct stream *stream, struct list_head *head);

/* read buffer from a stream */
int stream_read(struct stream *stream, struct list_head *head);

/* has a stream read eof */
static inline int stream_eof(struct stream *stream)
{
	return stream->eof;
}

#endif /* _STREAM_H */

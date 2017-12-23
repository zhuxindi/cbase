/**
 * File: stream_test.c
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#include <stream.h>
#include <log.h>
#include <systime.h>
#include <pool.h>
#include <unistd.h>
#include <fcntl.h>

static inline void set_nonblock(int fd)
{
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main()
{
	struct pool *pool;
	struct memops ops;
	struct stream rstream, wstream;
	struct list_head buf = LIST_HEAD_INIT(buf);
	const size_t rdbuf_len = 32;

	update_pid();
	update_sys_time();
	set_log_level(LOG_DEBUG);
	event_init(10);

	pool = pool_create(rdbuf_len + BUFFER_HEADER_SIZE);
	if (!pool)
		return 1;

	pool_set_memops(pool, &ops);

	stream_init(&rstream, STREAM_T_RD, rdbuf_len, &ops);
	set_nonblock(0);
	stream_attach_fd(&rstream, 0);
	event_wait();
	stream_read(&rstream, &buf);
	stream_detach(&rstream);

	stream_init(&wstream, STREAM_T_WR, 0, &ops);
	set_nonblock(1);
	stream_attach_fd(&wstream, 1);
	event_wait();
	stream_write(&wstream, &buf);
	stream_detach(&wstream);

	event_deinit();
	pool_destroy(pool);
	return 0;
}

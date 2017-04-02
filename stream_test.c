/**
 * File: stream_test.c
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#include <stream.h>
#include <log.h>
#include <systime.h>
#include <unistd.h>
#include <fcntl.h>

static inline void set_nonblock(int fd)
{
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main()
{
	struct stream rstream, wstream;
	struct list_head buf = LIST_HEAD_INIT(buf);

	set_log_level(LOG_DEBUG);
	update_sys_time();
	event_init(10);

	stream_init(&rstream, STREAM_T_RD, 32, 0);
	set_nonblock(0);
	stream_attach_fd(&rstream, 0);
	event_wait();
	stream_read(&rstream, &buf);
	stream_detach(&rstream);

	stream_init(&wstream, STREAM_T_WR, 32, 0);
	set_nonblock(1);
	stream_attach_fd(&wstream, 1);
	event_wait();
	stream_write(&wstream, &buf);
	stream_detach(&wstream);

	event_deinit();
	return 0;
}

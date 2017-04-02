/**
 * File: event.c
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#include <event.h>
#include <log.h>
#include <utils.h>
#include <sys/epoll.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define EPOLL_RD	(EPOLLIN|EPOLLRDHUP)
#define EPOLL_WR	EPOLLOUT
#define EPOLL_RDWR	(EPOLL_RD|EPOLL_WR)

static const char *event_names[] = {
	[EVENT_T_READ]	= "read",
	[EVENT_T_WRITE]	= "write",
	[EVENT_T_TIMER]	= "timer",
};

/* epoll events */
static int epfd = -1;
static struct epoll_event *events;
static int maxevents;

/* timer events */
static struct rbtree timer_rbtree;
static struct rbtree_node timer_rbtree_sentinel;

int event_init(int max_events)
{
	if (max_events <= 0)
		max_events = 1;

	/* create epoll fd */
	epfd = epoll_create(1);
	if (epfd == -1) {
		log_error("epoll_create() error: %s", strerror(errno));
		return -1;
	}

	/* allocate epoll event array */
	maxevents = max_events;
	events = calloc(maxevents, sizeof(struct epoll_event));
	if (!events) {
		log_error("malloc() error: %s", strerror(errno));
		close(epfd);
		return -1;
	}

	/* init timer event tree */
	rbtree_init(&timer_rbtree, &timer_rbtree_sentinel, rbtree_insert_value);

	log_debug("init events max_events %d", maxevents);
	return 0;
}

void event_deinit(void)
{
	log_debug("deinit events");
	close(epfd);
	free(events);
}

static inline unsigned long event_now(void)
{
	return (unsigned long)time_of_day.tv_sec * 1000 +
		(unsigned long)time_of_day.tv_usec / 1000;
}

int event_process_timer(int limit)
{
	unsigned long now = event_now();
	int n = 0;

	/* iterate timer tree to process timeouted timers */
	while (n < limit) {
		struct rbtree_node *min = rbtree_min(&timer_rbtree);

		if (min == &timer_rbtree_sentinel)
			break;

		if (min->key <= now) {
			struct event *ev = container_of(min, struct event, node);

			rbtree_delete(&timer_rbtree, min);
			ev->active = 0;
			ev->handler(ev->data);
			n++;
			continue;
		}

		break;
	}

	return n;
}

int event_process_io(int limit)
{
	int n;

	/* process epoll events */
	for (n = 0; n < limit; n++) {
		struct event *ev = events[n].data.ptr;

		/* write event ready */
		if (events[n].events & (EPOLL_WR|EPOLLERR|EPOLLHUP)) {
			if (ev->active && ev->type == EVENT_T_WRITE)
				ev->handler(ev->data);
			else if (ev->buddy && ev->buddy->active &&
				 ev->buddy->type == EVENT_T_WRITE)
				ev->buddy->handler(ev->buddy->data);
		}

		/* read event ready */
		if (events[n].events & (EPOLL_RD|EPOLLERR|EPOLLHUP)) {
			if (ev->active && ev->type == EVENT_T_READ)
				ev->handler(ev->data);
			else if (ev->buddy && ev->buddy->active &&
				 ev->buddy->type == EVENT_T_READ)
				ev->buddy->handler(ev->buddy->data);
		}
	}

	return n;
}

int event_wait(void)
{
	struct rbtree_node *min = rbtree_min(&timer_rbtree);
	unsigned long now = event_now();
	int timeout = -1, n;

	/* have timer events, then get the latest timeout */
	if (min != &timer_rbtree_sentinel)
		timeout = (min->key > now) ? (int)(min->key - now) : 0;

	/* waiting for events coming */
	log_debug("waiting events timeout %d", timeout);
	n = epoll_wait(epfd, events, maxevents, timeout);

	/* update system time */
	if (timeout != 0)
		update_sys_time();

	if (n == -1) {
		if (errno != EINTR) {
			log_error("epoll_wait() error: %s", strerror(errno));
			return -1;
		}

		log_debug("epoll_wait() stopped by a signal");
		n = 0;
	}

	/* process epoll events before timer events */
	if (n > 0)
		n = event_process_io(n);

	/* process timer events */
	n += event_process_timer(maxevents - n);

	if (n > 0)
		log_debug("processed %d events", n);
	return n;
}

static int event_add_timer(struct event *ev)
{
	if (ev->active && event_del(ev) < 0)
		return -1;

	/* set key to timeout and insert tree */
	ev->node.key = event_now() + ev->interval;
	rbtree_insert(&timer_rbtree, &ev->node);

	/* active event */
	ev->active = 1;
	log_debug("register %s event %p done interval %lums",
		  event_names[EVENT_T_TIMER], ev, ev->interval);
	return 0;
}

static int event_add_io(struct event *ev)
{
	struct epoll_event ee;
	int op = EPOLL_CTL_ADD;
	uint32_t events = (ev->type == EVENT_T_READ) ? EPOLL_RD : EPOLL_WR;

	if (ev->active)
		return 0;

	/* ev has buddy and buddy is active */
	if (ev->buddy && ev->buddy->active) {
		/* buddy is not an io event */
		if (ev->buddy->type != EVENT_T_READ &&
		    ev->buddy->type != EVENT_T_WRITE) {
			log_warn("event %p and buddy %p type conflict",
				 ev, ev->buddy);
			goto no_buddy;
		}

		/* fd is not same */
		if (ev->fd != ev->buddy->fd) {
			log_warn("event %p and buddy %p fd conflict %d %d",
				 ev, ev->buddy, ev->fd, ev->buddy->fd);
			goto no_buddy;
		}

		/* type is same */
		if (ev->type == ev->buddy->type) {
			log_warn("event %p is the same with buddy %p, ignored",
				 ev, ev->buddy);
			goto outer;
		}

		op = EPOLL_CTL_MOD;
		events = EPOLL_RDWR;
	}

no_buddy:
	/* add event to epoll */
	ee.events = events | EPOLLET;
	ee.data.ptr = ev;
	log_debug("%s epoll event fd %d", op == EPOLL_CTL_MOD ? "mod" : "add", ev->fd);
	if (epoll_ctl(epfd, op, ev->fd, &ee) == -1) {
		log_error("epoll_ctl() error: %s", strerror(errno));
		return -1;
	}

outer:
	ev->active = 1;
	log_debug("register %s event %p done fd %d",
		  event_names[ev->type], ev, ev->fd);
	return 0;
}

int event_add(struct event *ev)
{
	switch (ev->type) {
	case EVENT_T_TIMER:
		return event_add_timer(ev);
	case EVENT_T_READ:
	case EVENT_T_WRITE:
		return event_add_io(ev);
	default:
		log_error("invalid event type %d", ev->type);
		return -1;
	}

	/* never reached */
}

static int event_del_timer(struct event *ev)
{
	if (!ev->active)
		return 0;

	rbtree_delete(&timer_rbtree, &ev->node);
	ev->active = 0;
	log_debug("unregister %s event %p done", event_names[EVENT_T_TIMER], ev);
	return 0;
}

static int event_del_io(struct event *ev)
{
	struct epoll_event ee;
	int op = EPOLL_CTL_DEL;

	if (!ev->active)
		return 0;

	/* ev has buddy event and buddy is active */
	if (ev->buddy && ev->buddy->active) {
		/* buddy is not an io event */
		if (ev->buddy->type != EVENT_T_READ &&
		    ev->buddy->type != EVENT_T_WRITE) {
			log_warn("event %p and buddy %p type conflict",
				 ev, ev->buddy);
			goto no_buddy;
		}

		/* fd is not same */
		if (ev->fd != ev->buddy->fd) {
			log_warn("event %p and buddy %p fd conflict %d %d",
				 ev, ev->buddy, ev->fd, ev->buddy->fd);
			goto no_buddy;
		}

		if (ev->type == EVENT_T_READ && ev->buddy->type == EVENT_T_WRITE)
			ee.events = EPOLL_WR | EPOLLET;
		else if (ev->type == EVENT_T_WRITE && ev->buddy->type == EVENT_T_READ)
			ee.events = EPOLL_RD | EPOLLET;
		else {
			log_warn("event %p is the same with buddy %p, ignored",
				 ev, ev->buddy);
			goto outer;
		}

		ee.data.ptr = ev->buddy;
		op = EPOLL_CTL_MOD;
	}

no_buddy:
	/* remove event from epoll */
	log_debug("%s epoll event fd %d", op == EPOLL_CTL_MOD ? "mod" : "del", ev->fd);
	if (epoll_ctl(epfd, op, ev->fd, &ee) == -1) {
		log_error("epoll_ctl() error: %s", strerror(errno));
		return -1;
	}

outer:
	ev->active = 0;
	log_debug("unregister %s event %p done", event_names[ev->type], ev);
	return 0;
}

int event_del(struct event *ev)
{
	switch (ev->type) {
	case EVENT_T_TIMER:
		return event_del_timer(ev);
	case EVENT_T_READ:
	case EVENT_T_WRITE:
		return event_del_io(ev);
	default:
		log_error("invalid event type %d", ev->type);
		return -1;
	}

	/* never reached */
}

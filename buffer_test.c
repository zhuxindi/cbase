/**
 * File: buffer_test.c
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#include <buffer.h>
#include <log.h>
#include <systime.h>
#include <stdio.h>
#include <string.h>

static void *my_malloc(size_t n, void *unused)
{
	void *ptr = malloc(n);

	if (ptr)
		log_info("malloc(%lu): %p", n, ptr);
	else
		log_info("malloc(%lu) failed", n);
	return ptr;
}

static void my_free(void *ptr, void *unused)
{
	if (ptr) {
		log_info("free(%p)", ptr);
		free(ptr);
	}
}

void print_buffer(struct buffer *b)
{
	char *s;

	s = strndup(b->data, b->tail - b->data);
	log_info("%p datalen %ld freelen %ld data '%s'", b,
		 b->tail - b->data, b->end - b->tail, s);
	free(s);
}

int main()
{
	struct buffer_ops ops = { my_malloc, my_free, NULL };
	struct list_head head = LIST_HEAD_INIT(head);
	struct buffer *b, *b1, *b2;
	int i;

	update_pid();
	update_sys_time();
	set_log_level(LOG_DEBUG);

	b = buffer_create(20, &ops);
	if (!b)
		return 1;
	list_add_tail(&b->list, &head);

	for (i = 0; i < 16; i++)
		memcpy(b->tail++, "a", 1);
	print_buffer(b);

	b1 = buffer_separate(b, 10);
	if (!b1)
		return 1;
	print_buffer(b);
	print_buffer(b1);

	b2 = buffer_separate(b1, 5);
	if (!b2)
		return 1;
	print_buffer(b1);
	print_buffer(b2);

	for (i = 0; b2->end > b2->tail; i++)
		memcpy(b2->tail++, "b", 1);
	print_buffer(b2);

	buffer_release_chain(&head);
	return 0;
}

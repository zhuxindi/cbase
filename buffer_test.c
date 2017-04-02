/**
 * File: buffer_test.c
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#include <buffer.h>
#include <log.h>
#include <stdio.h>
#include <string.h>

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
	struct list_head head = LIST_HEAD_INIT(head);
	struct buffer *b, *b1, *b2;
	int i;

	update_sys_time();

	b = buffer_alloc(20, 0);
	if (!b)
		return 1;
	list_add_tail(&b->list, &head);

	for (i = 0; i < 16; i++)
		memcpy(b->tail++, "a", 1);
	print_buffer(b);

	b1 = buffer_separate(b, 10, 0);
	if (!b1)
		return 1;
	print_buffer(b1);

	b2 = buffer_separate(b1, 5, 0);
	if (!b2)
		return 1;
	print_buffer(b2);

	for (i = 0; b2->end > b2->tail; i++)
		memcpy(b2->tail++, "b", 1);
	print_buffer(b2);

	buffer_free_chain(&head);
	return 0;
}

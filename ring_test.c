/**
 * File: ring_test.c
 * Author: ZhuXindi
 * Date: 2017-12-22
 */

#include <ring.h>
#include <systime.h>
#include <log.h>

DEFINE_RING(static_ring, 8);

static void test_ring(struct ring *ring)
{
	size_t n;
	char buf[1024];

	n = ring_push(ring, "helloworld", 10);
	log_info("ring_push(): %lu", n);

	n = ring_pop(ring, buf, 1024);
	log_info("ring_pop(): %lu", n);

	n = ring_push(ring, "helloworld", 10);
	log_info("ring_push(): %lu", n);

	n = ring_pop(ring, buf, 1024);
	log_info("ring_pop(): %lu", n);	
}
	
int main()
{
	struct ring *ring;

	update_pid();
	update_sys_time();
	set_log_level(LOG_DEBUG);

	if ((ring = ring_create(8)) == NULL)
		return 1;

	test_ring(&static_ring);
	test_ring(ring);

	ring_destroy(ring);
	return 0;
}
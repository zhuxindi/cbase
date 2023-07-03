/**
 * File: ring_test.c
 * Author: ZhuXindi
 * Date: 2017-12-22
 */

#include <cbase/ring.h>
#include <cbase/systime.h>
#include <cbase/log.h>

DEFINE_RING(static_ring, 20);

static void test_ring(struct ring *ring)
{
	size_t n;
	char buf[1024];

	log_info("ring_size(): %lu", ring_size(ring));
	log_info("ring_used(): %lu, ring_usable(): %lu", ring_used(ring), ring_usable(ring));

	n = ring_push(ring, "helloworld", 10);
	log_info("ring_push(): %lu", n);
	log_info("ring_used(): %lu, ring_usable(): %lu", ring_used(ring), ring_usable(ring));

	n = ring_pop(ring, buf, 1);
	log_info("ring_pop(): %lu", n);
	log_info("ring_used(): %lu, ring_usable(): %lu", ring_used(ring), ring_usable(ring));

	n = ring_push(ring, "helloworld2", 11);
	log_info("ring_push(): %lu", n);
	log_info("ring_used(): %lu, ring_usable(): %lu", ring_used(ring), ring_usable(ring));

	n = ring_pop(ring, buf, 1024);
	log_info("ring_pop(): %lu", n);
	log_info("ring_used(): %lu, ring_usable(): %lu", ring_used(ring), ring_usable(ring));
}

int main()
{
	struct ring *ring;

	update_pid();
	update_sys_time();
	set_log_level(LOG_DEBUG);

	test_ring(&static_ring);

	ring = ring_create(8);
	test_ring(ring);
	ring_destroy(ring);

	return 0;
}

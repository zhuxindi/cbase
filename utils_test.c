/**
 * File: utils_test.c
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#include <utils.h>
#include <stdio.h>

int main()
{
	int x = 0, y = 3;
	int a[20];
	struct s { int a; } s = {3}, *ps;

	if (likely(x == 0))
		printf("x = %d\n", x);
	if (unlikely(y == 3))
		printf("y = %d\n", y);

	printf("a.length = %lu\n", ARRAY_SIZE(a));

	ps = container_of(&s.a, struct s, a);
	if (ps == &s)
		printf("&s = %p\n", ps);

	printf("min(%d,%d) = %d\n", x, y, min(x, y));
	printf("max(%d,%d) = %d\n", x, y, max(x, y));

	return 0;
}

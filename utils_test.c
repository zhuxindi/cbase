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
	struct s { int a; } s;

	if (likely(x == 0))
		printf("x = %d\n", x);
	if (unlikely(y == 3))
		printf("y = %d\n", y);

	printf("ARRAY_SIZE(int[20]) = %lu\n", ARRAY_SIZE(a));

	printf("ALIGN(0, 8) = %d\n", ALIGN(0, 8));
	printf("ALIGN(3, 8) = %d\n", ALIGN(3, 8));
	printf("ALIGN(8, 8) = %d\n", ALIGN(8, 8));
	printf("ALIGN(11, 8) = %d\n", ALIGN(11, 8));

	printf("container_of(&s.a) = %p, &s = %p\n", container_of(&s.a, struct s, a), &s);

	printf("min(%d, %d) = %d\n", x, y, min(x, y));
	printf("max(%d, %d) = %d\n", x, y, max(x, y));

	return 0;
}

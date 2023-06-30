/**
 * File: strings_test.c
 * Author: ZhuXindi
 * Date: 2017-06-19
 */

#include <xstrings.h>
#include <stdio.h>

int main()
{
	const char s[] = " Hello World! ";
	char buf[100], buf2[100];
	int n, m;

	printf("origin: \"%s\"\n", s);

	strcpy(buf, s);
	*strtrim(buf) = 0;
	printf("strtrim(): \"%s\"\n", buf);

	strcpy(buf, s);
	*strtoupper(buf) = 0;
	printf("strtoupper(): \"%s\"\n", buf);

	strcpy(buf, s);
	*strtolower(buf) = 0;
	printf("strtolower(): \"%s\"\n", buf);

	strcpy(buf, s);
	*strntoupper(buf,6) = 0;
	printf("strntoupper(6): \"%s\"\n", buf);

	strcpy(buf, s);
	*strntolower(buf,6) = 0;
	printf("strntolower(6): \"%s\"\n", buf);

	printf("strcasechr('O'): \"%s\"\n", strcasechr(s, 'O'));

	printf("strnchr('o', 6): \"%s\"\n", strnchr(s, 'o', 6));
	printf("strnchr('o', 3): \"%s\"\n", strnchr(s, 'o', 3));

	printf("strncasechr('O', 6): \"%s\"\n", strncasechr(s, 'O', 6));
	printf("strncasechr('O', 3): \"%s\"\n", strncasechr(s, 'O', 3));

	printf("strnstr('o', 6): \"%s\"\n", strnstr(s, "o", 6));
	printf("strnstr('o', 3): \"%s\"\n", strnstr(s, "o", 3));

	printf("strncasestr(\"O\", 6): \"%s\"\n", strncasestr(s, "O", 6));
	printf("strncasestr(\"O\", 3): \"%s\"\n", strncasestr(s, "O", 3));

	printf("strstrn(\"ok\", 1): \"%s\"\n", strstrn(s, "ok", 1));
	printf("strcasestrn(\"OK\", 1): \"%s\"\n", strcasestrn(s, "OK", 1));

	*hexdump(buf, s, strlen(s)) = 0;
	printf("hexdump(): \"%s\"\n", buf);

	n = base64_encoded_len(strlen(s));
	printf("base64_encoded_len(%d): %d\n", (int)strlen(s), n);
	m = base64_decoded_len(n);
	printf("base64_decoded_len(%d): %d\n", n, m);

	n = base64_encode(buf, s, strlen(s)) - buf;
	printf("base64_encode(): \"%.*s\"\n", n, buf);
	m = base64_decode(buf2, buf, n) - buf2;
	printf("base64_decode(): \"%.*s\"\n", m, buf2);

	n = base64_encode_url(buf, s, strlen(s)) - buf;
	printf("base64_encode_url(): \"%.*s\"\n", n, buf);
	m = base64_decode_url(buf2, buf, n) - buf2;
	printf("base64_decode_url(): \"%.*s\"\n", m, buf2);

	return 0;
}

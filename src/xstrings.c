/**
 * File: xstrings.c
 * Author: ZhuXindi
 * Date: 2017-06-19
 */

#include <cbase/xstrings.h>
#include <ctype.h>

char *strtrim(char *s)
{
	char *d = s;
	size_t n = strlen(s);

	while (n && isspace(s[n-1]))
		n--;
	while (isspace(*s)) {
		s++;
		n--;
	}

	memmove(d, s, n);
	return d + n;
}

char *strtoupper(char *s)
{
	while (*s) {
        	*s = toupper(*s);
		s++;
	}
	return s;
}

char *strntoupper(char *s, size_t n)
{
	while (*s && n--) {
        	*s = toupper(*s);
		s++;
	}
	return s;
}

char *strtolower(char *s)
{
	while (*s) {
        	*s = tolower(*s);
		s++;
	}
	return s;
}

char *strntolower(char *s, size_t n)
{
	while (*s && n--) {
        	*s = tolower(*s);
		s++;
	}
	return s;
}

char *strcasechr(const char *s, int c)
{
	c = tolower(c);

	while (*s) {
        	if (tolower(*s) == c)
			return (char *)s;
		s++;
	}
	return NULL;
}

char *strnchr(const char *s, int c, size_t n)
{
	while (*s && n--) {
		if (*s == c)
			return (char *)s;
		s++;
	}
	return NULL;
}

char *strncasechr(const char *s, int c, size_t n)
{
	c = tolower(c);

	while (*s && n--) {
		if (tolower(*s) == c)
			return (char *)s;
		s++;
	}
	return NULL;
}

char *strnstr(const char *s1, const char *s2, size_t n)
{
	int c = *s2++;
	char *p;

	while (1) {
		p = strnchr(s1, c, n);
		if (!p)
			return NULL;

		n -= ++p - s1;
		s1 = p;
		if (!strncmp(s1, s2, n))
			break;
	}
	return (char *)--s1;
}

char *strstrn(const char *s1, const char *s2, size_t n)
{
	int c = *s2++;
	char *p;

	n--;
	while (1) {
		p = strchr(s1, c);
		if (!p)
			return NULL;

		s1 = ++p;
		if (!strncmp(s1, s2, n))
			break;
	}
	return (char *)--s1;
}

char *strncasestr(const char *s1, const char *s2, size_t n)
{
	int c = *s2++;
	char *p;

	while (1) {
		p = strncasechr(s1, c, n);
		if (!p)
			return NULL;

		n -= ++p - s1;
		s1 = p;
		if (!strncasecmp(s1, s2, n))
			break;
	}
	return (char *)--s1;
}

char *strcasestrn(const char *s1, const char *s2, size_t n)
{
	int c = *s2++;
	char *p;

	n--;
	while (1) {
		p = strcasechr(s1, c);
		if (!p)
			return NULL;

		s1 = ++p;
		if (!strncasecmp(s1, s2, n))
			break;
	}
	return (char *)--s1;
}

char *hexdump(char *d, const char *src, size_t n)
{
	static const char hex_chars[] = "0123456789abcdef";
	const unsigned char *s = (const unsigned char *)src;

	while (n--) {
		*d++ = hex_chars[*s >> 4];
		*d++ = hex_chars[*s++ & 0xf];
	}
	return d;
}

static char *__base64_encode(char *d, const char *src, size_t n, const char *b64, int pad)
{
	const unsigned char *s = (const unsigned char *)src;

	while (n > 2) {
		*d++ = b64[(s[0] >> 2) & 0x3f];
		*d++ = b64[((s[0] & 3) << 4) | (s[1] >> 4)];
		*d++ = b64[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
		*d++ = b64[s[2] & 0x3f];
		s += 3;
		n -= 3;
	}

	if (n) {
		*d++ = b64[(s[0] >> 2) & 0x3f];
		if (n == 1) {
			*d++ = b64[(s[0] & 3) << 4];
			if (pad)
				*d++ = '=';
		} else {
			*d++ = b64[((s[0] & 3) << 4) | (s[1] >> 4)];
			*d++ = b64[(s[1] & 0x0f) << 2];
		}
		if (pad)
			*d++ = '=';
	}
	return d;
}

char *base64_encode(char *dst, const char *src, size_t n)
{
	static const char b64[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	return __base64_encode(dst, src, n, b64, 1);
}

char *base64_encode_url(char *dst, const char *src, size_t n)
{
	static const char b64[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

	return __base64_encode(dst, src, n, b64, 0);
}

static char *__base64_decode(char *d, const char *src, size_t n, const unsigned char *b64)
{
	const unsigned char *s = (const unsigned char *)src;
	size_t len;

	for (len = 0; len < n; len++) {
		if (s[len] == '=')
			break;
		if (b64[s[len]] == 77)
			return NULL;
	}

	if (len % 4 == 1)
		return NULL;

	while (len > 3) {
		*d++ = b64[s[0]] << 2 | b64[s[1]] >> 4;
		*d++ = b64[s[1]] << 4 | b64[s[2]] >> 2;
		*d++ = b64[s[2]] << 6 | b64[s[3]];
		s += 4;
		len -= 4;
	}
	if (len > 1)
		*d++ = b64[s[0]] << 2 | b64[s[1]] >> 4;
	if (len > 2)
		*d++ = b64[s[1]] << 4 | b64[s[2]] >> 2;
	return d;
}

char *base64_decode(char *dst, const char *src, size_t n)
{
	static const unsigned char b64[] = {
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 62, 77, 77, 77, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 77, 77, 77, 77, 77, 77,
		77,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 77, 77, 77, 77, 77,
		77, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77
	};

	return __base64_decode(dst, src, n, b64);
}

char *base64_decode_url(char *dst, const char *src, size_t n)
{
	static const unsigned char b64[] = {
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 62, 77, 77,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 77, 77, 77, 77, 77, 77,
		77,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 77, 77, 77, 77, 63,
		77, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
		77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77
	};

	return __base64_decode(dst, src, n, b64);
}

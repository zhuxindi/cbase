/**
 * File: strings.h
 * Author: ZhuXindi
 * Date: 2017-06-19
 */

#ifndef _XSTRINGS_H
#define _XSTRINGS_H

#include <string.h>

/* string modifiers */
char *strtrim(char *s);
char *strtoupper(char *s);
char *strntoupper(char *s, size_t n);
char *strtolower(char *s);
char *strntolower(char *s, size_t n);

/* string searchers, return first match postion or NULL if not found */
char *strcasechr(const char *s, int c);
char *strnchr(const char *s, int c, size_t n);
char *strncasechr(const char *s, int c, size_t n);
char *strnstr(const char *s1, const char *s2, size_t n);
char *strstrn(const char *s1, const char *s2, size_t n);
char *strncasestr(const char *s1, const char *s2, size_t n);
char *strcasestrn(const char *s1, const char *s2, size_t n);

/* dump every byte of src in hex, ensure dst has at least n*2 bytes */
char *hexdump(char *dst, const char *src, size_t n);

/* base64 */
#define base64_encoded_len(n)  (((n) + 2) / 3 * 4)
#define base64_decoded_len(n)  (((n) + 3) / 4 * 3)

char *base64_encode(char *dst, const char *src, size_t n);
char *base64_decode(char *dst, const char *src, size_t n);
char *base64_encode_url(char *dst, const char *src, size_t n);
char *base64_decode_url(char *dst, const char *src, size_t n);

#endif /* _XSTRINGS_H */

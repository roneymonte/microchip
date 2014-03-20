#ifndef _ENIDE_STRING_H
#define _ENIDE_STRING_H

unsigned char *memcpy(unsigned char *dest, unsigned char *src, unsigned int n)
{
#ifdef _MEMCPY_FORWARD
	unsigned int i;
	for (i = 0; i < n; i++)
		dest[i] = src[i];
#else
	while (n--)
		dest[n] = src[n];
#endif
	return dest;
}

#endif

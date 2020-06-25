#include <cli_hush.h>

typedef long long int size_t;
#define NULL (0)

#ifndef NOLIBS
/* memory allocation/deallocation routines */
void dlfree(void *mem) { return free(mem); }
void * dlmalloc(size_t bytes) { return malloc(bytes); }
void * dlrealloc(void *old, size_t bytes) { return realloc(old, bytes); }
void putc(char c) { write(1,&c,1); }
// u-boot code assumes puts does not add a newline
int  puts(char *str) { write(1, str, strlen(str)); return 1; }
#else
/* memory allocation/deallocation routines */
void dlfree(void *mem) { return; }
void * dlmalloc(size_t bytes) { return 0; }
void * dlrealloc(void *old, size_t bytes) { return 0; }

#endif


int start_hush(void) { return   u_boot_hush_start(); }


/* console dependecies */
/* test if ctrl-c was pressed */
int ctrlc(void) { return 0; }
int had_ctrlc (void) { return 0; }
void clear_ctrlc(void) { return; }

/* cli interface routines */
char console_buffer[4096];	/* console I/O buffer	*/
int cli_readline(const char *const prompt) {
/*
 * If console_buffer isn't 0-length the user will be prompted to modify
 * it instead of entering it from scratch as desired.
	 */
	console_buffer[0] = '\0';

return -1; // return length or error
}

/* command has a dependency of a default boot command */
int do_bootd(void *cmdtp, int flag, int argc, char *const argv[])
{
  return -1;
}

unsigned long get_timer(unsigned long base) {
  return 0;
}

/* env stuff needs to be figured out */

/**
 * env_complete() - return an auto-complete for environment variables
 *
 * @var: partial name to auto-complete
 * @maxv: Maximum number of matches to return
 * @cmdv: Returns a list of possible matches
 * @maxsz: Size of buffer to use for matches
 * @buf: Buffer to use for matches
 * @dollar_comp: non-zero to wrap each match in ${...}
 * @return number of matches found (in @cmdv)
 */
int env_complete(char *var, int maxv, char *cmdv[], int maxsz, char *buf,
		 int dollar_comp) { 
  return 0; 
}

char *env_get(const char *varname) { return NULL; }

#ifndef NOLIBS

char *sandbox_strdup(const char *s) { return strdup(s); }

#else
// this is other dependences if you don't have standard libs
/* misc */
/**
 * strcpy - Copy a %NUL terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 */
char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

/**
 * strlen - Find the length of a string
 * @s: The string to be sized
 */
size_t strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}


char * sandbox_strdup(const char *s)
{
	char *new;

	if ((s == NULL)	||
	    ((new = dlmalloc (strlen(s) + 1)) == NULL) ) {
		return NULL;
	}

	strcpy (new, s);
	return new;
}

/**
 * strcmp - Compare two strings
 * @cs: One string
 * @ct: Another string
 */
int strcmp(const char * cs,const char * ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}

/**
 * strchr - Find the first occurrence of a character in a string
 * @s: The string to be searched
 * @c: The character to search for
 */
char * strchr(const char * s, int c)
{
	for(; *s != (char) c; ++s)
		if (*s == '\0')
			return NULL;
	return (char *) s;
}

/**
 * strncpy - Copy a length-limited, %NUL-terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 * @count: The maximum number of bytes to copy
 *
 * Note that unlike userspace strncpy, this does not %NUL-pad the buffer.
 * However, the result is not %NUL-terminated if the source exceeds
 * @count bytes.
 */
char * strncpy(char * dest,const char *src,size_t count)
{
	char *tmp = dest;

	while (count-- && (*dest++ = *src++) != '\0')
		/* nothing */;

	return tmp;
}

/**
 * strcat - Append one %NUL-terminated string to another
 * @dest: The string to be appended to
 * @src: The string to append to it
 */
char * strcat(char * dest, const char * src)
{
	char *tmp = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++) != '\0')
		;

	return tmp;
}

/**
 * strncat - Append a length-limited, %NUL-terminated string to another
 * @dest: The string to be appended to
 * @src: The string to append to it
 * @count: The maximum numbers of bytes to copy
 *
 * Note that in contrast to strncpy, strncat ensures the result is
 * terminated.
 */
char * strncat(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	if (count) {
		while (*dest)
			dest++;
		while ((*dest++ = *src++)) {
			if (--count == 0) {
				*dest = '\0';
				break;
			}
		}
	}

	return tmp;
}

/**
 * strncmp - Compare two length-limited strings
 * @cs: One string
 * @ct: Another string
 * @count: The maximum number of bytes to compare
 */
int strncmp(const char * cs,const char * ct,size_t count)
{
	register signed char __res = 0;

	while (count) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
		count--;
	}

	return __res;
}

/**
 * memset - Fill a region of memory with the given value
 * @s: Pointer to the start of the area.
 * @c: The byte to fill the area with
 * @count: The size of the area.
 *
 * Do not use memset() to access IO space, use memset_io() instead.
 */
void * memset(void * s,int c,size_t count)
{
	unsigned long *sl = (unsigned long *) s;
	char *s8;
	s8 = (char *)sl;
	while (count--)
		*s8++ = c;

	return s;
}

/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
void * memcpy(void *dest, const void *src, size_t count)
{
	unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
	char *d8, *s8;

	if (src == dest)
		return dest;

	/* while all data is aligned (common case), copy a word at a time */
	if ( (((unsigned long)dest | (unsigned long)src) & (sizeof(*dl) - 1)) == 0) {
		while (count >= sizeof(*dl)) {
			*dl++ = *sl++;
			count -= sizeof(*dl);
		}
	}
	/* copy the reset one byte at a time */
	d8 = (char *)dl;
	s8 = (char *)sl;
	while (count--)
		*d8++ = *s8++;

	return dest;
}

/**
 * memcmp - Compare two areas of memory
 * @cs: One area of memory
 * @ct: Another area of memory
 * @count: The size of the area.
 */
int memcmp(const void * cs,const void * ct,size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}


/* stdio */
int puts(const char *s) { return 0; }
typedef void * FILE;
int putc(int c, FILE *stream) { return (int)((unsigned char)c); }
int printf(const char *format, ...) { return 0; }
int sprintf(char *str, const char *format, ...) { return 0; }
#endif

int
main(int argc, char **argv)
{
  int i;
  int rc = start_hush();
  int hush_flags = FLAG_PARSE_SEMICOLON | FLAG_EXIT_FROM_LOOP;

  rc = parse_string_outer("x=7", hush_flags);
  printf("rc=%d\n", rc);
  if (argc>1) {
    for (i=1; i<argc; i++) {
      rc = parse_string_outer(argv[i], hush_flags);
      printf("rc=%d\n", rc);
    }
  }
  return rc;
}

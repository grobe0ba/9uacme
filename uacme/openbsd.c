/* The following functions have been ripped out of the OpenBSD libc,
 * and then brutally abused. Any and all problems and awfulness are
 * entirely my fault.
 * Thanks guys!
 * -- grobe0ba
 */

/*-
 * Copyright (c) 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#include "openbsd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/limits.h>

size_t
strnlen(const char *str, size_t maxlen)
{
        const char *cp;

        for (cp = str; maxlen != 0 && *cp != '\0'; cp++, maxlen--)
                ;

        return (size_t)(cp - str);
}

char *
strndup(const char *str, unsigned long maxlen)
{
        char *copy;
        unsigned long len;

        len = strnlen(str, maxlen);
        copy = malloc(len + 1);
        if (copy != NULL) {
                (void)memcpy(copy, str, len);
                copy[len] = '\0';
        }

        return copy;
}

void
vwarnx(const char *fmt, va_list ap)
{
        (void)fprintf(stderr, "%s: ", __progname);
        if (fmt != NULL)
                (void)vfprintf(stderr, fmt, ap);
        (void)fprintf(stderr, "\n");
}

void
warnx(const char *fmt, ...)
{
        va_list ap;

        va_start(ap, fmt);
        vwarnx(fmt, ap);
        va_end(ap);
}

void
vwarn(const char *fmt, va_list ap)
{
	int sverrno;

	(void)fprintf(stderr, "%s: ", __progname);
	if (fmt != NULL) {
		(void)vfprintf(stderr, fmt, ap);
		(void)fprintf(stderr, ": ");
	}
}

void
warn(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vwarn(fmt, ap);
	va_end(ap);
}

#define INITIAL_SIZE    128

int vasprintf(char **str, char *fmt, va_list args) {
	int ret;
	char *buf;

	if((buf = calloc(16384, sizeof(char))) == NULL) {
		return -1;
	}
	
	ret = vsprintf(buf, fmt, args);

	if (ret == -1) {
		free(buf);
		return -1;
	}
	
	if((*str = calloc(ret+1, sizeof(char))) == NULL) {
		free(buf);
		return -1;
	}

	memcpy(*str, buf, ret+1);

	free(buf);
	
	return ret;
}

int
asprintf(char **str, const char *fmt, ...)
{
	int ret;
	va_list ap;

	va_start(ap, fmt);
	ret = vasprintf(str, fmt, ap);
	va_end(ap);

	return ret;
}

char *
basename(char *path)
{
        static char bname[PATH_MAX];
        size_t len;
        const char *endp, *startp;

        /* Empty or NULL string gets treated as "." */
        if (path == NULL || *path == '\0') {
                bname[0] = '.';
                bname[1] = '\0';
                return (bname);
        }

        /* Strip any trailing slashes */
        endp = path + strlen(path) - 1;
        while (endp > path && *endp == '/')
                endp--;

        /* All slashes becomes "/" */
        if (endp == path && *endp == '/') {
                bname[0] = '/';
                bname[1] = '\0';
                return (bname);
        }

        /* Find the start of the base */
        startp = endp;
        while (startp > path && *(startp - 1) != '/')
                startp--;

        len = endp - startp + 1;
        if (len >= sizeof(bname)) {
                return (NULL);
        }
        memcpy(bname, startp, len);
        bname[len] = '\0';
        return (bname);
}

char *
dirname(char *path)
{
        static char dname[PATH_MAX];
        size_t len;
        const char *endp;

        /* Empty or NULL string gets treated as "." */
        if (path == NULL || *path == '\0') {
                dname[0] = '.';
                dname[1] = '\0';
                return (dname);
        }

        /* Strip any trailing slashes */
        endp = path + strlen(path) - 1;
        while (endp > path && *endp == '/')
                endp--;

        /* Find the start of the dir */
        while (endp > path && *endp != '/')
                endp--;

        /* Either the dir is "/" or there are no slashes */
        if (endp == path) {
                dname[0] = *endp == '/' ? '/' : '.';
                dname[1] = '\0';
                return (dname);
        } else {
                /* Move forward past the separating slashes */
                do {
                        endp--;
                } while (endp > path && *endp == '/');
        }

        len = endp - path + 1;
        if (len >= sizeof(dname)) {
                return (NULL);
        }
        memcpy(dname, path, len);
        dname[len] = '\0';
        return (dname);
}

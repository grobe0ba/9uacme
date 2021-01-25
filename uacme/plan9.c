#include <u.h>
#include <libc.h>
#include <stdio.h>
#include <regexp.h>

#include "plan9.h"

typedef struct acme {
    void *key;
    void *json;
    void *account;
    void *dir;
    void *order;
    char *nonce;
    char *kid;
    char *headers;
    char *body;
    char *type;
    unsigned char alt_fp[32];
    ulong alt_fp_len;
    ulong alt_n;
    const char *eab_keyid;
    const char *eab_key;
    const char *directory;
    const char *hook;
    const char *email;
    char *keyprefix;
    char *certprefix;
} acme_t;

char *find_header(const char *headers, const char *name)
{
    char regex[512];
	Reprog *reg;
	Resub m[2];
	char *ret;

    if (snprintf(regex, 512, "^%s:[ \t]*(.*)\r\n", name) < 0) {
        warnx("find_header: asprintf failed");
        return nil;
    }
	
	if((reg = regcomp(regex)) == nil) {
		warnx("find_header: regcomp failed");
	} else {
		if(regexec(reg, headers, m, 2) == 1) {
			if(! (ret = strndup(m[1].sp, m[1].ep - m[1].sp))) {
				warnx("find_header: strndup failed");
			}
		}
	}
	free(reg);
	return ret;
}

int eab_parse(acme_t *a, char *eab) {
	Resub m[3];
	Reprog *reg;

	if((reg = regcomp("^([^:]+):([-_A-Za-z0-9]+)$")) == nil) {
		warnx("eab_parse: regcomp failed");
		return 0;
	}

	if(!regexec(reg, eab, m, 3)) {
		warnx("-e credentials must be specified as 'KEYID:KEY', with KEY base64url encoded");
		return 1;
	}

	m[1].ep = 0;
	a->eab_keyid = m[1].sp;

	m[2].ep = 0;
	a->eab_key = m[2].sp;

	return 1;
}


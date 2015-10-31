/************************************************************************
* intl.cpp
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2013-2015 <lisa@ltmnet.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#ifdef _WIN32
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <windows.h>
#endif

#include "intl.h"
#include "path.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

typedef struct intl_s {
	void *mo_data;
	int inv_endian;
	int str_count;
	int tbl_offset[2];
	int hash_count;
	int hash_offset;
	uint32_t plurals;
	char* plural;
} intl_t;

#define INTL_MO_MAGIC 0x950412DE
#define INTL_MO_MAGIC_REV 0xDE120495
#define INTL_ORIG 0
#define INTL_TRANS 1

/* remove whitespace from beginning and end of a string */
static char* intl_trim(char* str)
{
	int32_t l;

	while (*str && isspace(*str)) {
		str++;
	}

	l = strlen(str);
	while (--l > -1 && (!str[l] || isspace(str[l]))) {
		str[l] = 0;
	}

	return str;
}

static uint32_t intl_eval_preceed(char* str, char* p, char** b)
{
	int mul = 1;
	uint32_t r = 0;
	while (isdigit(*p) || (mul == 1 && isspace(*p))) {
		r += ((*p)-48)*mul;
		mul *= 10;
		if (p == str)
			break;
		p--;
	}
	if (p != str)
		p++;
	*b = p;
	return r;
}

/* returns 1 if str contains only a number */
static uint32_t intl_eval_check(char* str)
{
	char buff[50];
	uint32_t v = strtoul(str,NULL,10);
	if (!str || !str[0])
		return 1;
	sprintf(buff,"%u",v);
	if (!strcmp(buff,str))
		return 1;
	return 0;
}

/* evaluate a plurals expression to an integer value */
static uint32_t intl_eval(char* str)
{
	char buff[1024];
	char* qc;
	char* cc;
	char* ac;
	char* oc;
	str = intl_trim(str);
	if (intl_eval_check(str))
		return strtoul(str,NULL,10);

	if (str[0] == '(') {
		int o = 1;
		int c = 0;
		uint32_t r;
		int i;
		for (i=1; str[i]; i++) {
			switch (str[i]) {
			case '(':
				o++;
				break;
			case ')':
				c++;
				break;
			default:;
			}
			if (o == c)
				break;
		}
		c = str[i];
		str[i] = 0;
		r = intl_eval(str+1);
		str[i] = c;
		if (c) {
			sprintf(buff,"%u%s",r,str+i+1);
		}else{
			sprintf(buff,"%u",r);
		}
		if (intl_eval_check(buff))
			return strtoul(buff,NULL,10);
		return intl_eval(buff);
	}

	qc = strchr(str,'?');
	if (qc) {
		int o = 1;
		int c = 0;
		uint32_t r;
		int i;
		for (i=1; qc[i]; i++) {
			switch (qc[i]) {
			case '?':
				o++;
				break;
			case ':':
				c++;
				break;
			default:;
			}
			if (o == c)
				break;
		}
		if (o != c)
			return 0;
		cc = qc+i;
		*qc = 0;
		r = intl_eval(str);
		*qc = '?';
		if (r) {
			*cc = 0;
			strcpy(buff,qc+1);
			*cc = ':';
		}else{
			strcpy(buff,cc+1);
		}
		if (intl_eval_check(buff))
			return strtoul(buff,NULL,10);
		return intl_eval(buff);
	}

	ac = strstr(str,"&&");
	oc = strstr(str,"||");
	if (ac && (!oc || ac < oc)) {
		uint32_t r;
		*ac = 0;
		r = intl_eval(str);
		*ac = '&';
		if (!r)
			return 0;
		strcpy(buff,ac+2);
		if (intl_eval_check(buff))
			return strtoul(buff,NULL,10);
		return intl_eval(buff);
	}

	if (oc) {
		uint32_t r;
		*oc = 0;
		r = intl_eval(str);
		*oc = '|';
		if (r)
			return 1;
		strcpy(buff,oc+2);
		if (intl_eval_check(buff))
			return strtoul(buff,NULL,10);
		return intl_eval(buff);
	}

	ac = strchr(str,'=');
	if (ac) {
		uint32_t r1;
		uint32_t r2;
		uint32_t r3 = 0;
		char* e;
		char* n;
		char ec;
		if (*(ac-1) == '!') {
			e = ac-1;
			n = ac+1;
		}else if (*(ac-1) == '>') {
			e = ac-1;
			n = ac+1;
		}else if (*(ac-1) == '<') {
			e = ac-1;
			n = ac+1;
		}else if (*(ac+1) == '=') {
			e = ac;
			n = ac+2;
		}else{
			return 0;
		}
		ec = *e;
		*e = 0;
		r1 = intl_eval(str);
		r2 = intl_eval(n);
		*e = ec;
		switch (ec) {
		case '!':
			r3 = (r1 != r2);
			break;
		case '>':
			r3 = (r1 >= r2);
			break;
		case '<':
			r3 = (r1 <= r2);
			break;
		case '=':
			r3 = (r1 == r2);
			break;
		}
		return r3;
	}

	ac = strchr(str,'<');
	if (ac) {
		uint32_t r1;
		uint32_t r2;
		*ac = 0;
		r1 = intl_eval(str);
		r2 = intl_eval(ac+1);
		*ac = '<';
		return (r1<r2);
	}

	ac = strchr(str,'>');
	if (ac) {
		uint32_t r1;
		uint32_t r2;
		*ac = 0;
		r1 = intl_eval(str);
		r2 = intl_eval(ac+1);
		*ac = '>';
		return (r1>r2);
	}

	ac = strchr(str,'*');
	oc = strchr(str,'/');
	if (!ac && !oc) {
		ac = strchr(str,'%');
		if (!ac) {
			ac = strchr(str,'+');
			oc = strchr(str,'-');
			if (oc && (!ac || oc < ac))
				ac = oc;
		}
	}else if (oc && (!ac || oc < ac)) {
		ac = oc;
	}
	if (ac) {
		uint32_t r1;
		uint32_t r2;
		uint32_t r3 = 0;
		char* b;
		char* e;

		if (ac == str)
			return 0;

		r1 = intl_eval_preceed(str,ac-1,&b);
		r2 = strtoul(ac+1,&e,10);
		switch(*ac) {
		case '*':
			r3 = (r1*r2);
			break;
		case '/':
			if (!r2) {
				r3 = 0;
			}else{
				r3 = (r1/r2);
			}
			break;
		case '%':
			if (!r2) {
				r3 = 0;
			}else{
				r3 = (r1%r2);
			}
			break;
		case '+':
			r3 = (r1+r2);
			break;
		case '-':
			r3 = (r1-r2);
			break;
		}

		if (b != str) {
			char bc = *b;
			*b = 0;
			sprintf(buff,"%s%u%s",str,r3,e);
			*b = bc;
		}else if (*e) {
			sprintf(buff,"%u%s",r3,e);
		}else{
			return r3;
		}
		if (intl_eval_check(buff))
			return strtoul(buff,NULL,10);
		return intl_eval(buff);
	}

	if (intl_eval_check(str))
		return strtoul(str,NULL,10);

	return 0;
}

/* get the plural index based on n */
static uint32_t intl_getplural(intl_t *intl, int n)
{
	char buff[1024];
	char nb[50];
	int i;
	int k;
	int o;
	uint32_t r;

	if (!intl->plural)
		return 0;

	sprintf(nb,"%d",n);

	for (o=0,i=7; intl->plural[i]; i++) {
		if (o > 1022)
			return 0;
		if (intl->plural[i] == 'n') {
			for (k=0; nb[k]; k++) {
				buff[o++] = nb[k];
				if (o > 1022)
					return 0;
			}
			continue;
		}
		buff[o++] = intl->plural[i];
	}
	buff[o] = 0;

	r = intl_eval(buff);
	if (r >= intl->plurals)
		return 0;
	return r;
}

/* basic hashpjw used by lots of things, including mo files */
static uint32_t intl_hash(const char *str_param)
{
	uint32_t hval = 0;
	uint32_t g;
	const char *s = str_param;

	while (*s) {
		hval <<= 4;
		hval += (unsigned char) *s++;
		g = hval & ((uint32_t) 0xf << 28);
		if (g != 0) {
			hval ^= g >> 24;
			hval ^= g;
		}
	}

	return hval;
}

/* read a 32 bit int with correct endianness */
static uint32_t intl_readint(intl_t *intl, int offset)
{
	uint8_t *p;
	uint32_t r = *((uint32_t*)(((char*)intl->mo_data) + offset));

	if (!intl->inv_endian)
		return r;

	p = (uint8_t*)&r;
	return (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
}

/* get a string from either the original strings table INTL_ORIG
 * or the translated strings tables INTL_TRANS */
static char* intl_getstr(intl_t *intl, int type, int index, int *length)
{
	uint32_t offset = intl->tbl_offset[type] + (8 * index);
	*length = intl_readint(intl,offset);
	offset = intl_readint(intl,offset+4);

	return ((char *)intl->mo_data) + offset;
}

/* get the language code (en_GB etc) */
static void intl_getlang(char* buff, int size)
{
#ifdef _WIN32
	char l1[4];
	char l2[4];

	if (!GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SISO639LANGNAME, (LPSTR)l1, 4))
		strcpy(l1,"en");
	if (!GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SISO3166CTRYNAME , (LPSTR)l2, 4))
		l2[0] = 0;

	if (l1[0] && l2[0]) {
		snprintf(buff,size,"%s_%s",l1,l2);
	}else if (l1[0]) {
		strncpy(buff,l1,size);
	}else{
		strncpy(buff,"en",size);
	}
#else
	char* lang = getenv("LANG");
	if (!lang || !lang[0])
		lang = (char*)"en";
	strncpy(buff,lang,size);
#endif
}

/* essentially gettext() but with a per-mo-file option */
char* intl_lookup(intl_t *intl, char *s, int *length)
{
	uint32_t h;
	int hp;
	int hp_o;
	int inc;
	uint32_t i;
	char* l;
	int len;

	if (!intl || !intl->mo_data)
		return s;

	h = intl_hash(s);
	hp = h % intl->hash_count;
	hp_o = hp;
	inc = 1 + (h % (intl->hash_count - 2));

	while (1) {
		i = intl->hash_offset+(4*hp);
		i = intl_readint(intl, i);
		if (!i)
			break;

		/* entries in the table are stored +1 so that 0 means empty */
		i--;

		l = intl_getstr(intl,INTL_ORIG,i,&len);
		if (!strcmp(l,s)) {
			l = intl_getstr(intl,INTL_TRANS,i,&len);
			if (length)
				*length = len;
			return l;
		}

		hp += inc;
		hp %= intl->hash_count;

		if (hp == hp_o)
			break;
	}

	if (length)
		*length = strlen(s);
	return s;
}

/* as per intl_lookup, but for ngettext() */
char* intl_nlookup(intl_t *intl, char* s1, char* s2, int n)
{
	char* s = s1;
	int p;
	int i;
	int length;
	int sl;
	int cl;
	int tl;
	char* l;
	char* c = NULL;

	if (!s2)
		return intl_lookup(intl,s1,NULL);

	if (!intl || !intl->mo_data) {
		if (n == 1)
			return s1;
		return s2;
	}

	l = intl_lookup(intl,s,&length);
	if (intl->plurals < 2)
		return l;
	sl = strlen(l);
	if (sl == length)
		return l;

	p = intl_getplural(intl,n);
	if (!p)
		return l;

	tl = sl;

	for (i=0; i<p; i++) {
		if (tl >= length)
			return l;
		c = l+tl+1;
		cl = strlen(c);
		tl += cl+1;
	}

	return c;
}

/* initialise an intl_t by loading in an mo file */
int intl_init(intl_t *intl, const char* file)
{
	char fbuff[2048];
	char lbuff[128];
	FILE *f;
	int length;
	uint32_t magic;
	char* head;
	char* p;
	char* e;
	std::string path;

	intl_getlang(lbuff,128);

	intl->mo_data = NULL;
	intl->str_count = 0;
	intl->tbl_offset[INTL_ORIG] = 0;
	intl->tbl_offset[INTL_TRANS] = 0;
	intl->hash_count = 0;
	intl->hash_offset = 0;
	intl->plurals = 1;
	intl->plural = NULL;

	path = std::string("translation-")+lbuff;
	path = getPath(path.c_str(),file,true);
	if (path == "") {
		char* u = strchr(lbuff,'_');
		if (!u)
			return 1;
		*u = 0;

		path = std::string("translation-")+lbuff;
		path = getPath(path.c_str(),file,true);
		if (path == "")
			return 1;
	}

	f = fopen(path.c_str(), "rb");
	if (!f)
		return 2;

	fseek(f, 0, SEEK_END);
	length = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (length < 24) {
		fclose(f);
		return 3;
	}

	intl->mo_data = malloc(length);
	if (!intl->mo_data) {
		fclose(f);
		return 4;
	}

	if (length != (int)fread(intl->mo_data, 1, length, f)) {
		fclose(f);
		free(intl->mo_data);
		intl->mo_data = NULL;
		return 5;
	}
	fclose(f);

	magic = ((uint32_t*)intl->mo_data)[0];

	if (magic == INTL_MO_MAGIC) {
		intl->inv_endian = 0;
	}else if (magic == INTL_MO_MAGIC_REV) {
		intl->inv_endian = 1;
	}else{
		free(intl->mo_data);
		intl->mo_data = NULL;
		return 6;
	}

	intl->str_count = intl_readint(intl, 8);
	intl->tbl_offset[INTL_ORIG] = intl_readint(intl, 12);
	intl->tbl_offset[INTL_TRANS] = intl_readint(intl, 16);
	intl->hash_count = intl_readint(intl, 20);
	intl->hash_offset = intl_readint(intl, 24);

	if (!intl->hash_count) {
		free(intl->mo_data);
		intl->mo_data = NULL;
		return 7;
	}

	head = intl_lookup(intl,(char*)"",NULL);
	if (!head)
		return 0;

	p = strstr(head,"Plural-Forms:");
	if (!p)
		return 0;

	head = p;
	e = strchr(head,'\n');
	if (e)
		*e = 0;

	strncpy(fbuff,head+13,2048);
	if (e)
		*e = '\n';
	head = fbuff;

	p = strstr(head,"nplurals=");
	if (!p)
		return 0;
	e = strchr(p,';');
	if (e)
		*e = 0;

	intl->plurals = strtol(p+9,NULL,10);
	if (e)
		*e = ';';

	p = strstr(head,"plural=");
	if (!p)
		return 0;
	e = strchr(p,';');
	if (e)
		*e = 0;

	intl->plural = strdup(p);
	if (e)
		*e = ';';

	return 0;
}

/* the normal stuff */
static intl_t intl;
char* gettext(const char *s)
{
	return intl_lookup(&intl,(char*)s,NULL);
}
char* ngettext(const char* s1, const char* s2, int n)
{
	return intl_nlookup(&intl,(char*)s1,(char*)s2,n);
}

wchar_t *mb2wc(const char *src)
{
#ifdef _WIN32
	static wchar_t *w = (wchar_t*)L"";
	int ol = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
	if (!ol)
		return w;
	wchar_t *buff = new wchar_t[ol];
	if (!MultiByteToWideChar(CP_UTF8, 0, src, -1, buff, ol))
		return w;
	return buff;
#else
	int l = strlen(src)+1;
	wchar_t *buff = new wchar_t[l];
	mbstate_t state;
	memset(&state, '\0', sizeof (state));
	size_t n = mbsrtowcs(buff, &src, l, &state);
	buff[n] = L'\0';
	return buff;
#endif
}

wchar_t* wgettext(const char *str)
{
	return mb2wc(intl_lookup(&intl,(char*)str,NULL));
}

wchar_t* wngettext(const char *str1, const char *str2, int n)
{
	return mb2wc(intl_nlookup(&intl,(char*)str1,(char*)str2,n));
}

void init_gettext()
{
#ifndef _WIN32
	setlocale(LC_MESSAGES, "");
	setlocale(LC_CTYPE, "");
#endif
	intl_init(&intl,"voxelands.mo");
#ifndef SERVER
	init_KeyNamesLang();
#endif
}

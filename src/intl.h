#ifndef GETTEXT_HEADER
#define GETTEXT_HEADER
#include <iostream>

char* gettext(const char *s);
char* ngettext(const char* s1, const char* s2, int n);
void init_gettext();

inline wchar_t* chartowchar_t(const char *str)
{
	size_t l = strlen(str)+1;
	wchar_t* nstr = new wchar_t[l];
	mbstowcs(nstr, str, l);
	return nstr;
}

wchar_t* wgettext(const char *str);
wchar_t* wngettext(const char *str1, const char *str2, int n);

#ifndef SERVER
// Initialise KeyNamesLang array
void init_KeyNamesLang();
#endif

#endif

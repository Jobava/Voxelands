#ifndef GETTEXT_HEADER
#include "config.h" // for USE_GETTEXT
#include <iostream>

#if USE_GETTEXT
#include <libintl.h>
#else
#define gettext(String) String
#define ngettext(String1,String2,Int) String2
#endif

#define _(String) gettext(String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)

inline void init_gettext(const char *path) {
#if USE_GETTEXT
	// don't do this if MSVC compiler is used, it gives an assertion fail
	#ifndef _MSC_VER
		setlocale(LC_MESSAGES, "");
		setlocale(LC_CTYPE, "");
	#endif
	bindtextdomain(PROJECT_NAME, path);
	textdomain(PROJECT_NAME);
#endif
}

inline wchar_t* chartowchar_t(const char *str)
{
	size_t l = strlen(str)+1;
	wchar_t* nstr = new wchar_t[l];
	mbstowcs(nstr, str, l);
	return nstr;
}

inline wchar_t* wgettext(const char *str)
{
	wchar_t *r = chartowchar_t(gettext(str));
	return r;
}

inline wchar_t* wngettext(const char *str1, const char *str2, int n)
{
	wchar_t *r = chartowchar_t(ngettext(str1,str2,n));
	return r;
}
#define GETTEXT_HEADER
#endif

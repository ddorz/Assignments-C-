/* ======================================================================================================================= *
 * Dave Dorzback                                                                                                           *
 * strings.h                                                                                                               *
 *                                                                                                                         *
 * This header defines a number of string related functions, including functions for manipulating and analysing strings.   *
 * Macro versions are also included for most functions, and are named identically but proceeded by an underscore.          *
 * ======================================================================================================================= */

#ifndef strings_h
#define strings_h

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#if defined(__SIZE_TYPE__)
    typedef __SIZE_TYPE__ __darwin_size_t;
#else
    typedef unsigned long int __darwin_size_t;
#endif

#ifndef _SIZE_T
#define _SIZE_T
    typedef __darwin_size_t size_t;
#endif

#if defined(__PTRDIFF_TYPE__)
    typedef __PTRDIFF_TYPE__ ptrdiff_t;
#else
    typedef signed long int __PTRDIFF_TYPE__;
#endif

#ifndef _PTRDIFF_T
#define _PTRDIFF_T
    #if !defined(_GCC_PTRDIFF_T)
    #define _GCC_PTRDIFF_T
        typedef __PTRDIFF_TYPE__ ptrdiff_t;
    #else 
        typedef __PTRDIFF_TYPE__ ptrdiff_t;
    #endif
#endif

#ifndef NULL
    #ifdef __cplusplus
        #if !defined(__MINGW32__) && !defined(_MSC_VER)
            #define NULL __null
        #else
            #define NULL 0
        #endif
    #else
        #define NULL ((void*)0)
    #endif
#endif

#ifndef stdbool_h
    #if !defined(__cplusplus) && !defined(_Bool)
        #define bool _Bool
        #define boolean _Bool
        #define true 1
        #define false 0
    #elif defined(__GNUC__) && !defined(__STRICT_ANSI__)
        #define _Bool bool
        #define bool bool
        #define _Bool boolean
        #define boolean boolean
        #define false false
        #define true true
    #endif
#endif

#define EXTENDED_ASCII_RANGE 256

typedef struct {
    int bit:1;
} BitArray;

#define uc(s) (s > 0x60 && s < 0x7b ? s&0x5F : s)
#define toUpper(s) us(s)

#define lc(s) (s > 0x40 && s < 0x5b ? s|0x60 : s)
#define toLower(s) lc(s)

/* *************************************************************************************************************************/
#define strUpr_(str) ({ for (register size_t i = 0; (str[i] = uc(str[i])); ++i); })

extern inline char *strUpr(char *str);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strLwr_(str) ({ for (register size_t i = 0; (str[i] = lc(str[i])); ++i); })

extern inline char *strLwr(char *str);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strLen_(str) ({                   \
    char *s;                              \
    for (s = str; *s && *++s; ++s);       \
    s-str;                                \
})

extern inline size_t strLen(const char *str);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCmp_(s1, s2) ({                                      \
    register size_t i;                                          \
    for (i = 0; s1[i] == s2[i] && s1[i]; ++i);                  \
    (int)(s1[i]-s2[i]);                                         \
})

extern inline int strCmp(const char *s1, const char *s2);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCaseCmp_(s1, s2) ({                             \
    register size_t i;                                     \
    for (i = 0; lc(s1[i]) == lc(s2[i]) && s1[i]; ++i);     \
    (int)(lc(s1[i])-lc(s2[i]));                            \
})

extern inline int strCaseCmp(const char *s1, const char *s2);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strnCmp_(s1, s2, n) ({                             \
    register long int i;                                   \
    for (i = 0; i < n && s1[i] && s1[i]==s2[i]; ++i);      \
    (int)(s1[i]-s2[i]);                                    \
})

extern inline int strnCmp(const char *s1, const char *s2, long int n);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strnCaseCmp_(s1, s2, n) ({                              \
    register long int i;                                        \
    for (i = 0; i < n && s1[i] && lc(s1[i])==lc(s2[i]); ++i);   \
    (int)(lc(s1[i])-lc(s2[i]));                                 \
})

extern inline int strnCaseCmp(const char *s1, const char *s2, long int n);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCpy_(dest, src) ({                             \
    for (register size_t i = 0; (dest[i] = src[i]); ++i); \
    dest;                                                 \
})

extern inline ptrdiff_t strCpy(char *restrict dest, const char *restrict source);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCaseCpy_(dest, src) ({                             \
    for (register size_t i = 0; (dest[i] = lc(src[i])); ++i); \
    dest;                                                     \
})

extern inline ptrdiff_t strCaseCpy(char *restrict dest, const char *restrict source);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strnCpy_(dest, src, n) ({                               \
    for (register size_t i = 0; i < n; ++i)                     \
        dest[i] = src[i];                                       \
    dest;                                                       \
})

extern inline ptrdiff_t strnCpy(char *restrict dest, const char *restrict source, size_t n);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strnCaseCpy_(dest, src, n) ({                           \
    for (register size_t i = 0; i < n; ++i)                     \
        dest[i] = lc(src[i]);                                   \
    dest;                                                       \
})

extern inline ptrdiff_t strnCaseCpy(char *restrict dest, const char *restrict source, size_t n);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCat_(dest, src) ({                           \
    register size_t i, j;                               \
    for (i = 0; dest[i]; ++i);                          \
    for (j = 0; (dest[i] = src[j]); ++i, ++j);          \
    dest;                                               \
})

extern inline ptrdiff_t strCat(char *restrict dest, const char *restrict source);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCaseCat_(dest, src) ({                       \
    register size_t i, j;                               \
    for (i = 0; dest[i]; ++i);                          \
    for (j = 0; (dest[i] = lc(src[j])); ++i, ++j);      \
    dest;                                               \
})

extern inline ptrdiff_t strCaseCat(char *restrict dest, const char *restrict source);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCharCat_(dest, src) ({ for (char *s = !dest ? NULL : *dest ? dest + 1 : dest; s && (*s || (!(*s = src) && src)); ++s); dest; })


extern inline char *strCharCat(char *dest, char src);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCaseCharCat_(dest, src) ({ for (char *s = !dest ? NULL : *dest ? dest + 1 : dest; s && (*s || (!(*s = lc(src)) && src)); ++s); dest; })

extern inline char *strCaseCharCat(char *dest, char src);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strChr_(str, ch) ({                             \
    register size_t i;                                  \
    for (i = 0; str[i]!=ch && (str[i] || !ch); ++i);    \
    str[i]==ch ? &str[i] : NULL;                        \
})

extern inline char *strChr(const char *str, unsigned char ch);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCaseChr_(str, ch) ({                                                \
    register size_t i = 0;                                                     \
    for (unsigned char ch = lc(c); lc(str[i])!=ch && (str[i] || !ch); ++i);    \
    lc(str[i])==lc(c) ? &str[i] : NULL;                                        \
})

extern inline char *strCaseChr(const char *str, unsigned char ch);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strReverse_(str)                                                       \
do {                                                                           \
    register size_t n;                                                         \
    strLen_(str, n);                                                           \
    for (register size_t i = n>1 ? 0 : n; i < n/2; ++i)                        \
        str[i] ^= str[n-1-i] ^= str[i] ^= str[n-1-i];                          \
} while (0)

extern inline char *strReverse(char *str);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strnReverse_(dest, src, n)                                                  \
do {                                                                                \
    for (char *s1 = dest, *s2 = src + n - 1; *s2 && s1 < dest + n; *s1++ = *s2--);  \
} while (0)

extern inline char *strnReverse(char *restrict dest, char *restrict source, size_t n);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strMove_(dest, src, n)                                          \
do {                                                                    \
    for (register size_t i = (n && dest && src) ? 0 : n; i < n; ++i)    \
        dest[i] = src[i];                                               \
} while (0)

extern inline char *strMove(char *dest, char *source, size_t n);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCaseMove_(dest, src, n)                                      \
do {                                                                    \
    for (register size_t i = (n && dest && src) ? 0 : n; i < n; ++i)    \
        dest[i] = lc(src[i]);                                           \
} while (0)

extern inline char *strCaseMove(char *dest, char *source, size_t n);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strStr_(str, sub) ({                            \
    register unsigned short state = 0;                  \
    register size_t pos = 0, matched = 0;               \
    while (str[pos] && sub[matched]) {                  \
        switch (state) {                                \
            case 0:                                     \
                if (str[pos]==sub[matched])             \
                    ++matched, state = 1;               \
                else state = 2;                         \
            break;                                      \
            case 1:                                     \
                if (str[pos]==sub[matched])             \
                    ++matched;                          \
                else matched = 0, state = 2;            \
            break;                                      \
            case 2:                                     \
                if (str[pos]==sub[matched])             \
                    ++matched, state = 1;               \
            break;                                      \
        }                                               \
        ++pos;                                          \
    }                                                   \
    str[pos] ? &str[pos-matched] : NULL;                \
})

extern inline char *strStr(char *str, char *sub);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCaseStr_(str, sub) ({                        \
    register unsigned short state = 0;                  \
    register size_t pos = 0, matched = 0;               \
    while (str[pos] && sub[matched]) {                  \
        switch (state) {                                \
            case 0:                                     \
                if (lc(str[pos])==lc(sub[matched]))     \
                    ++matched, state = 1;               \
                else state = 2;                         \
            break;                                      \
            case 1:                                     \
                if (lc(str[pos])==lc(sub[matched]))     \
                    ++matched;                          \
                else matched = 0, state = 2;            \
            break;                                      \
            case 2:                                     \
                if (lc(str[pos])==lc(sub[matched]))     \
                    ++matched, state = 1;               \
            break;                                      \
        }                                               \
        ++pos;                                          \
    }                                                   \
    str[pos] ? &str[pos-matched] : NULL;                \
})

extern inline char *strCaseStr(char *str, char *sub);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strAllStr_(str, sub) ({                                                                                                                 \
    register size_t state = 0, idx = 0, matched = 0, needles_found = 0, substr_space = 5;                                                       \
    char **sub_strings = NULL;                                                                                                                  \
    while (str[idx]) {                                                                                                                          \
        switch (state) {                                                                                                                        \
            case 0:                                                                                                                             \
                if (str[idx] == sub[matched] && ++matched) state = 1;                                                                           \
                else state = 2;                                                                                                                 \
            break;                                                                                                                              \
            case 1:                                                                                                                             \
                if (str[idx] != sub[matched] && sub[matched]) {                                                                                 \
                    state = 2, matched = 0;                                                                                                     \
                } else if (!sub[matched++] || !sub[matched]) {                                                                                  \
                    char **tmp_substrs = realloc(sub_strings,(++needles_found==substr_space ? substr_space*=2 : substr_space)*sizeof(char*));   \
                    if (tmp_substrs) {                                                                                                          \
                        sub_strings = tmp_substrs;                                                                                              \
                        sub_strings[needles_found-1] = &str[idx-matched+1], sub_strings[needles_found] = NULL;                                  \
                    } else break;                                                                                                               \
                    state = 2, matched = 0;                                                                                                     \
                }                                                                                                                               \
            break;                                                                                                                              \
            case 2:                                                                                                                             \
                if (str[idx] == sub[matched] && ++matched) state = 1;                                                                           \
            break;                                                                                                                              \
        }                                                                                                                                       \
        ++idx;                                                                                                                                  \
    }                                                                                                                                           \
    substr_space != needles_found ? realloc(sub_strings, needles_found * sizeof(char *)) : sub_strings;                                         \
})                                                                                                                                              \

extern inline char **strAllStr(char *str, char *sub);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCaseAllStr_(str, sub) ({                                                                                                             \
    register size_t state = 0, idx = 0, matched = 0, needles_found = 0, substr_space = 5;                                                       \
    char **sub_strings = NULL;                                                                                                                  \
    while (str[idx]) {                                                                                                                          \
        switch (state) {                                                                                                                        \
            case 0:                                                                                                                             \
                state = (lc(str[idx]) == lc(sub[matched]) && ++matched) ? 1 : state;                                                            \
            break;                                                                                                                              \
            case 1:                                                                                                                             \
                if (lc(str[idx]) != lc(sub[matched]) && sub[matched])                                                                           \
                    state = 0, matched = 0;                                                                                                     \
                else if (!sub[matched++] || !sub[matched]) {                                                                                    \
                    char **tmp_ss = realloc(sub_strings,(++needles_found==substr_space ? substr_space*=2 : substr_space)*sizeof(char*));        \
                    if (tmp_ss) sub_strings = tmp_ss, sub_strings[needles_found-1] = &str[idx-matched+1], sub_strings[needles_found] = NULL;    \
                    else break;                                                                                                                 \
                    state = 0, matched = 0;                                                                                                     \
                }                                                                                                                               \
            break;                                                                                                                              \
        }                                                                                                                                       \
        ++idx;                                                                                                                                  \
    }                                                                                                                                           \
    substr_space != needles_found ? realloc(sub_strings, needles_found * sizeof(char *)) : sub_strings;                                         \
})

extern inline char **strCaseAllStr(char *str, char *sub);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strIsPal_(str) ({                                                                           \
    size_t n = strLen_(str);                                                                        \
    char end[(n/2)+1];                                                                              \
    for (char *mid = str + (n/2) - !(n%2), *s1 = end, *s2 = str + n - 1; s2 > mid; *s1++ = *s2--);  \
    (n < 2 || !strnCmp_(end,str,n/2)) ? true : false;                                               \
})


extern inline bool strIsPal(const char *str);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strCaseIsPal_(str) ({                                                                       \
    size_t n = strLen_(str);                                                                        \
    char end[(n/2)+1];                                                                              \
    for (char *mid = str + (n/2) - !(n%2), *s1 = end, *s2 = str + n - 1; s2 > mid; *s1++ = *s2--);  \
    (n < 2 || !strnCaseCmp_(end,str,n/2)) ? true : false;                                           \
})

extern inline bool strCaseIsPal(const char *str);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
#define strInsert_(s1, s2, idx) ({                                                                      \
    size_t n1, n2;                               /* n1 = length s1, n2 = length s2 */                   \
    char *p1, *p2;                                                                                      \
    for (p1 = (char*)s1; *p1 && *++p1; ++p1);    /* Loop to get length of s1 */                         \
    for (p2 = (char*)s2; *p2 && *++p2; ++p2);    /* Loop to get length of s2 */                         \
    n1 = p1-s1, n2 = p2-s2;                      /* 1st loop shifts s1 back, 2nd copies s2 into s1 */   \
    for (p1 = &s1[n1 + n2 - 1], p2 = &s1[n1 - 1], *p1 = *p2; p2 > &s1[idx]; *--p1 = *--p2);             \
    for (p1 = &s1[idx], p2 = s2, *p1 = *p2; p1 < &s1[idx] + n2 - 1; *++p1 = *++p2);                     \
    &s1[idx];                                                                                           \
})

extern inline char *strInsert(char *s1, const char *s2, size_t idx);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
// No macro versions

extern inline bool strIsPerm(const char *s1, const char *s2);
extern inline bool strCaseIsPerm(const char *s1, const char *s2);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
// No macro versions

extern inline bool strSort(char *str);
extern inline char *strCountSort(char *str);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
// No macro versions

extern inline bool strHasDups(char *str);
extern inline bool strCaseHasDups(char *str);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
// No macro version

extern inline size_t strPermutateAll(char *str, FILE *stream);
static inline size_t _permutate_no_duplicates(char *str, size_t n, FILE *stream, size_t *perm_count);
static inline size_t _permutate_has_duplicates(char *str, char *start, size_t n, FILE *stream, size_t *perm_count);
/* *************************************************************************************************************************/


/* *************************************************************************************************************************/
// No macro version

extern inline bool strCharCounts(char *str, FILE *stream);
/* *************************************************************************************************************************/

#endif /* strings_h */
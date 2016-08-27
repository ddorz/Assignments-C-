/* ======================================================================================================================= *
 * Dave Dorzback                                                                                                           *
 * strings.c                                                                                                               *
 * ======================================================================================================================= */

#include "strings.h"

/* =========================================================================================================================
 * _swap_iter - Iterator swap
 * ========================================================================================================================*/

#define _swap_iter(a, b) ({ char tmp = *a; *a = *b; *b = tmp; })


/* *************************************************************************************************************************
 * strUpr - Converts string to uppercase
 * *************************************************************************************************************************/

inline char *strUpr(char *str) {
    char *s = (str && *str) ? str : NULL;
    while ((*str = *str > 0x60 && *str < 0x7b ? *str&0x5F : *str) && ++str);
    return s;
}

/* *************************************************************************************************************************
 * strLwr - Converts string to lowercase
 * *************************************************************************************************************************/

inline char *strLwr(char *str) {
    char *s = (str && *str) ? str : NULL;
    while ((*str = *str > 0x40 && *str < 0x5b ? *str|0x60 : *str) && ++str);
    return s;
}

/* *************************************************************************************************************************
 * strLen - String length
 * *************************************************************************************************************************/

inline size_t strLen(const char *str) {
    char *s;
    for (s = (char*)str; *s && *++s; ++s);
    return (size_t)(s - str);
}

/* *************************************************************************************************************************
 * strCmp - String compare
 * *************************************************************************************************************************/

inline int strCmp(const char *s1, const char *s2) {
    for (; *s1 && *s1 == *s2; ++s1, ++s2);
    return (int)(*s1 - *s2);
}

/* *************************************************************************************************************************
 * strCaseCmp - String compare - Case insensitive
 * *************************************************************************************************************************/

inline int strCaseCmp(const char *s1, const char *s2) {
    for (; *s1 && lc(*s1) == lc(*s2); ++s1, ++s2);
    return (int)(lc(*s1) - lc(*s2));
}

/* *************************************************************************************************************************
 * strnCmp - String compare - First n characters
 * *************************************************************************************************************************/

inline int strnCmp(const char *s1, const char *s2, long int n) {
    for (; --n > 0 && *s1 && *s1 == *s2; ++s1, ++s2);               // If argument of n = 0 is used, 1st chars compared
    return (int)(*s1 - *s2);
}

/* *************************************************************************************************************************
 * strnCaseCmp - String compare - First n characters - Case insensitive
 * *************************************************************************************************************************/

inline int strnCaseCmp(const char *s1, const char *s2, long int n) {
    for (; --n > 0 && *s1 && lc(*s1) == lc(*s2); ++s1, ++s2);
    return (int)(lc(*s1) - lc(*s2));
}

/* *************************************************************************************************************************
 * strCpy - String copy - Returns number of characters copied. For string copy that returns pointer to destination string,
 * use macro version strCpy_
 * *************************************************************************************************************************/

inline ptrdiff_t strCpy(char *restrict dest, const char *restrict source) {
    char *s = dest;
    for (*dest = *source; *source; *++dest = *++source);
    return dest - s;
}

/* *************************************************************************************************************************
 * strCaseCopy - String copy - Case insensitive - Returns number of characters copied. For string case copy that returns
 * pointer to destination string, use macro version strCaseCopy_
 * *************************************************************************************************************************/

inline ptrdiff_t strCaseCpy(char *restrict dest, const char *restrict source) {
    char *s = dest;
    for (*dest = lc(*source); *source++; *++dest = lc(*source));
    return dest - s;
}

/* *************************************************************************************************************************
 * strnCpy - String copy - First n characters - Returns number of characters copied. For string n copy that returns pointer
 * to destination string, use macro version strnCpy_
 * *************************************************************************************************************************/

inline ptrdiff_t strnCpy(char *restrict dest, const char *restrict source, size_t n) {
    char *s;
    for (s = dest; n > 0; --n) *dest++ = *source++;
    return dest - s;
}

/* *************************************************************************************************************************
 * strnCaseCpy - String copy - First n characters - Case insensitive - Returns number of characters copied. For string n 
 * case copy that returns pointer to destination string, use macro version strnCaseCpy_
 * *************************************************************************************************************************/

inline ptrdiff_t strnCaseCpy(char *restrict dest, const char *restrict source, size_t n) {
    register size_t i;
    for (i = 0; i < n; ++i) dest[i] = lc(source[i]);
    return &dest[i] - dest;
}

/* *************************************************************************************************************************
 * strCat - String concatenation - Returns number of characters concatenated. For string concatenation that returns pointer
 * to destination string, use macro version strCat_
 * *************************************************************************************************************************/

inline ptrdiff_t strCat(char *restrict dest, const char *restrict source) {
    char *s;
    while (*dest++);
    for (s = --dest, *dest = *source; *source; *++dest = *++source);
    return dest - s;
}

/* *************************************************************************************************************************
 * strCaseCat - String case concatenation - Concatenates strings without case - Returns number of characters concatenated.
 * For string case concatenation that returns pointer to destination string, use macro version strCaseCat_
 * *************************************************************************************************************************/

inline ptrdiff_t strCaseCat(char *restrict dest, const char *restrict source) {
    char *s;
    while (*dest++);
    for (s = --dest, *dest = lc(*source); *source; *++dest = lc(*++source));
    return dest - s;
}

/* *************************************************************************************************************************
 * strCharCat - String character concatenation - Concatenates a single character - Returns pointer to destination string
 * *************************************************************************************************************************/

inline char *strCharCat(char *dest, char src) {
    for (char *s = !dest ? NULL : *dest ? dest + 1 : dest; s && (*s || (!(*s = src) && src)); ++s); // Verifies no NULL arguments
    return dest;
}


/* *************************************************************************************************************************
 * strCaseCharCat - String character concatenation - Case insensitive - Concatenates a single character
 * *************************************************************************************************************************/

inline char *strCaseCharCat(char *dest, char src) {
    for (char *s = !dest ? NULL : *dest ? dest + 1 : dest; s && (*s || (!(*s = lc(src)) && src)); ++s); // Verifies no NULL arguments
    return dest;
}


/* *************************************************************************************************************************
 * strChr - String character search - Returns position of character 'ch' in 'str'
 * *************************************************************************************************************************/

inline char *strChr(const char *str, unsigned char ch) {
    for (; *str!=ch && (*str || ch=='\0'); ++str);  // Search until match is found or end of string reached
    return *str==ch ? (char *)str : NULL;           // Return position of 'ch' in 'str' or NULL if no match found
}


/* *************************************************************************************************************************
 * strChr - String character search - Case insensitive - Returns position of character 'ch' in 'str'
 * *************************************************************************************************************************/

// Character sseach, case insensitive
inline char *strCaseChr(const char *str, unsigned char ch) {
    for (ch = lc(ch); lc(*str)!=ch && (*str || ch=='\0'); ++str); // Search until match is found or end of string reached
    return lc(*str)==ch ? (char *)str : NULL;                     // Return position of 'ch' in 'str' or NULL if no match found
}


/* *************************************************************************************************************************
 * strReverse - String reverse
 * *************************************************************************************************************************/

inline char *strReverse(char *str) {
    for (char *s1 = str, *s2 = s1 + strLen(str) - 1; s1 < s2; ++s1, --s2)   // Reverse str in place
        _swap_iter(s1,s2);                                                  // Swap elements
    return str;
}


/* *************************************************************************************************************************
 * strnReverse - String reverse - First n characters
 * *************************************************************************************************************************/

// Reverses the first n characters of the string 'source' and saves them to 'dest'
inline char *strnReverse(char *restrict dest, char *restrict source, size_t n) {
    for (char *s1 = dest, *s2 = source + n - 1; *s2 && s1 < dest + n; *s1++ = *s2--);
    return dest;
}


/* *************************************************************************************************************************
 * strMove - String move
 * *************************************************************************************************************************/

// Doesn't seem to be working with overlaps. Seems to have the same functionality as strnCpy?
inline char *strMove(char *dest, char *source, size_t n) {
    size_t i;
    char *s = (n && dest && source) ? dest : NULL;                  // Make sure all parameters are vaid and set s accordingly
    for (i = s ? 0 : n; i < n; ++i)                                 // If s is not null, copy first char to dest. Otherwise set i = n so loop never executes
        dest[i] = source[i];
    return s;
}


/* *************************************************************************************************************************
 * strCaseMove - String move - Case insensitive
 * *************************************************************************************************************************/

inline char *strCaseMove(char *dest, char *source, size_t n) {
    size_t i;
    char *s = (n && dest && source) ? dest : NULL;
    for (i = s ? 0 : n; i < n; ++i)
        dest[i] = source[i];
    return s;
}


/* *************************************************************************************************************************
 * strStr - String substring search
 * *************************************************************************************************************************/

inline char *strStr(char *str, char *sub) {
    
    enum {INITIAL, MATCH, MISMATCH} state = INITIAL;
    register size_t idx = 0, matched = 0;
    
    while (str[idx] && sub[matched]) {
        switch (state) {
            case INITIAL:
                if (str[idx]==sub[matched])
                    ++matched, state = MATCH;
                 else state = MISMATCH;
            break;
            case MATCH:
                if (str[idx]==sub[matched])
                    ++matched;
                else matched = 0, state = MISMATCH;
            break;
            case MISMATCH:
                if (str[idx]==sub[matched])
                    ++matched, state = MATCH;
            break;
        }
        ++idx;
    }
    return str[idx] ? &str[idx-matched] : NULL;
}


/* *************************************************************************************************************************
 * strCaseStr - String substring search - Case insensitive
 * *************************************************************************************************************************/

// Sub-string seach, case insensitive. Returns a pointer to the starting index of the given substring in 'str', or NULL if 'sub' is not a substring of 'str'.
inline char *strCaseStr(char *str, char *sub) {
    
    enum {INITIAL, MATCH, MISMATCH} state = INITIAL;
    register size_t idx = 0, matched = 0;
    
    while (str[idx] && sub[matched]) {              // Continues iterating until: a) the null char in str is reached or b) the substring has been found
        switch (state) {                            // Keeps track of the position in str and the number of characters matched using FSM
            case INITIAL:
                if (lc(str[idx])==lc(sub[matched])) // Initial State <=> If first element is a match, increment matched and set state = Match. If not, set state = Mismatch
                    ++matched, state = MATCH;
                else state = MISMATCH;
                break;
            case MATCH:                             // Match State <=> If current element is a match, increment matched. If not, set matched = 0 and state = Mismatch
                if (lc(str[idx])==lc(sub[matched]))
                    ++matched;
                else matched = 0, state = MISMATCH;
                break;
            case MISMATCH:                          // Mismatch State <=> If current element is a match, increment matched and set state = Match
                if (lc(str[idx])==lc(sub[matched]))
                    ++matched, state = MATCH;
                break;
        }
        ++idx;                                      // Move 1 position forward each iteration
    }
    return str[idx] ? &str[idx-matched] : NULL;     // If null character in str has not been reached, return the starting index where substring is located, otherwise return null.
}

/* =========================================================================================================================
 * _add_sstr_loc - Add substring location - Used by strAll functions
 * Adds address of substring to pointer array and allocates additional memory if needed
 * ========================================================================================================================*/

#define _add_sstr_loc(substrs,new_substring,pos)                                                                          \
do {                                                                                                                     \
    char **tmp_substrs = realloc(substrs, (pos == substr_space ? substr_space*=2 : substr_space) * sizeof(char *));      \
    if (tmp_substrs) {                                                                                                   \
        substrs = tmp_substrs;                                                                                           \
        substrs[needles_found-1] = new_substring;                                                                        \
        substrs[needles_found] = NULL;                                                                                   \
    } else {                                                                                                             \
        errno = 12;                                                                                                      \
        return substrs;                                                                                                  \
    }                                                                                                                    \
} while (0)

/* *************************************************************************************************************************
 * strAllStr - String substring search - All locations - Finds all substring locations in 'str' and saves locations to
 * an array of pointers. Returns first pointer in array
 * *************************************************************************************************************************/

inline char **strAllStr(char *str, char *sub) {
    
    enum {INITIAL, MATCH, MISMATCH} state = INITIAL;
    register size_t idx = 0, matched = 0, needles_found = 0, substr_space = 5;
    char **sub_strings = NULL;
    
    while (str[idx]) {
        switch (state) {
            case INITIAL:                                       // Initial State <=> If match is found, increment matched. Set state accordingly
                if (str[idx] == sub[matched] && ++matched)
                    state = MATCH;
                else state = MISMATCH;
            break;
            case MATCH:                                         // Match State <=> If current element not a match, set matched = 0. Otherwise, see if entire substring has been matched.
                if (str[idx] != sub[matched] && sub[matched]) {        // Second part of conditional added to handle length 1 substrings. If removed, substrings with len > 1 will be matched.
                    state = MISMATCH, matched = 0;
                } else if (!sub[matched++] || !sub[matched]) {         // This condition has also been modified to account for len 1 substrings, does not work for them if written as 'if (!sub[++matched])'
                    _add_sstr_loc(sub_strings, &str[idx-matched+1], ++needles_found);
                    state = MISMATCH, matched = 0;
                }
            break;
            case MISMATCH:                                      // Mismatch State <=> If the current element is a match, increment matched. Set state accordingly.
                if (str[idx] == sub[matched] && ++matched)
                    state = MATCH;
            break;
        }
        ++idx;
    }
    return substr_space != needles_found ? realloc(sub_strings, (needles_found+1) * sizeof(char *)) : sub_strings;
}


/* *************************************************************************************************************************
 * strCaseAllStr - String substring search - Case insensitive - All locations - Finds all substring locations in 'str' 
 * and saves locations to an array of pointers. Returns first pointer in array
 * *************************************************************************************************************************/

inline char **strCaseAllStr(char *str, char *sub) {
    
    enum {INITIAL, MATCH, MISMATCH} state = INITIAL;
    register size_t idx = 0, matched = 0, needles_found = 0, substr_space = 5;
    char **sub_strings = NULL;
    
    while (str[idx]) {
        switch (state) {
            case INITIAL:
                if (lc(str[idx])==lc(sub[matched]) && ++matched)
                    state = MATCH;
                 else state = MISMATCH;
            break;
            case MATCH:
                if (lc(str[idx]) != lc(sub[matched]) && sub[matched]) {
                    state = MISMATCH, matched = 0;
                } else if (!sub[matched++] || !sub[matched]) {
                    _add_sstr_loc(sub_strings, &str[idx-matched+1], ++needles_found);
                    state = MISMATCH, matched = 0;
                }
            break;
            case MISMATCH:
                if (lc(str[idx])==lc(sub[matched]) && ++matched)
                    state = MATCH;
            break;
        }
        ++idx;
    }
    return substr_space != needles_found+1 ? realloc(sub_strings, (needles_found+1) * sizeof(char *)) : sub_strings;
}


/* *************************************************************************************************************************
 * strIsPal - String is palindrome - Determines whether string is a palindrome or not
 * *************************************************************************************************************************/

inline bool strIsPal(const char *str) {
    size_t n = strLen_((char *)str);
    char end[(n/2)+1];
    for (char *mid = (char *)str + (n/2) - !(n%2), *s1 = end, *s2 = (char *)str + n - 1; s2 > mid; *s1++ = *s2--);
    return (n < 2 || !strnCmp_(end,str,n/2)) ? true : false;
}



/* *************************************************************************************************************************
 * strCaseIsPal - String is palindrome - Case insensitive - Determines whether string is a palindrome or not
 * *************************************************************************************************************************/

inline bool strCaseIsPal(const char *str) {
    size_t n = strLen_((char *)str);
    char end[(n/2)+1];
    for (char *mid = (char *)str + (n/2) - !(n%2), *s1 = end, *s2 = (char *)str + n - 1; s2 > mid; *s1++ = *s2--);
    return (n < 2 || !strnCaseCmp_(end,str,n/2)) ? true : false;
}


/* *************************************************************************************************************************
 * strInsert - String insert - Inserts s2 into s1 at the provided index idx
 * *************************************************************************************************************************/

inline char *strInsert(char *s1, const char *s2, size_t idx) {
    char *p1, *p2;
    for (p1 = s1; *p1 && *++p1; ++p1);                                                          // Loop until last element in s1
    for (p2 = (char*)s2; *p2 && *++p2; ++p2);                                                   // Loop until last element in s2
    size_t n1 = (size_t)(p1 - s1), n2 = (size_t)(p2 - s2);                                      // n1 = length s1, n2 = length s2
    for (p1 = &s1[n1 + n2 - 1], p2 = &s1[n1 - 1], *p1 = *p2; p2 > &s1[idx]; *--p1 = *--p2);     // Shift s1 backwards until insertion index is reached
    for (p1 = &s1[idx], *p1 = *s2; p1 < &s1[idx] + n2 - 1; *++p1 = *++s2);                      // Copy s2 into s1 starting at the insertion index
    return s1;
}


/* *************************************************************************************************************************
 * strIsPerm - String is permutation - Determines whether s1 and s2 are permutations
 * *************************************************************************************************************************/

inline bool strIsPerm(const char *s1, const char *s2) {
    
    // 256 buckets, one for each character. Initialize all buckets to 0
    size_t b1_[EXTENDED_ASCII_RANGE] = {0}, b2_[EXTENDED_ASCII_RANGE] = {0};

    // Count the occurance for each character in s1 and s2. Continue iterating until a null character is reached in either string
    while (*s1 && *s2 && ++b1_[*s1++] && ++b2_[*s2++]);
    
    // If the strings are of different length, return false
    if (*s1 || *s2) return false;
    
    // Compare occurance counts for each character. Return false if a discrepency is encountered
    for (unsigned int i = 0; i < EXTENDED_ASCII_RANGE; ++i) {
        if (b1_[i] != b2_[i]) return false;
    }
    
    return true;
}


/* *************************************************************************************************************************
 * strIsPerm - String is permutation - Case insensitive - Determines whether s1 and s2 are permutations
 * *************************************************************************************************************************/

inline bool strCaseIsPerm(const char *s1, const char *s2) {
    
    // 256 buckets, one for each character. Initialize all buckets to 0
    size_t b1_[EXTENDED_ASCII_RANGE] = {0}, b2_[EXTENDED_ASCII_RANGE] = {0};
    
    // Count the occurance for each character in s1 and s2. Continue iterating until a null character is reached in either string
    for (; *s1 && *s2; ++s1, ++s2) {
        ++b1_[lc(*s1)], ++b2_[lc(*s2)];
    }
    
    // If the strings are of different length, return false
    if (*s1 || *s2) return false;
    
    // Compare occurance counts for each character. Return false if a discrepency is encountered
    for (unsigned int i = 0; i < EXTENDED_ASCII_RANGE; ++i) {
        if (b1_[i] != b2_[i]) return false;
        
    }
    
    return true;
}

/* *************************************************************************************************************************
 * strSort - String sort - Sorts string using bubble sort
 * *************************************************************************************************************************/

inline bool strSort(char *str) {
    for (char *p = *str ? str+1 : str, swap = 0; *p; ++p) {
        for (char *q = str; *(q + (p-str)); ++q) {
            if (*q > *(q+1)) {
                _swap_iter(q,(q+1));
                swap = 1;
            }
        }
        if (!swap) break;
    }
    return str && *str ? true : false;
}


/* *************************************************************************************************************************
 * strSort - String sort - Alternative string sort function - Sorts string using count sort
 * *************************************************************************************************************************/

inline char *strCountSort(char *str) {
    // Create array for character counts and initialize all buckets to 0
    size_t counts[EXTENDED_ASCII_RANGE] = {0};
    
    // Iterate through str. For each occurence of given character, increment corresponding bucket
    char *s;
    for (s = str; *s; ) {
        ++counts[*s++];
    }
    s = str;
    
    // Iterate through buckets and save characters to str in order
    for (unsigned int i = 0; i < EXTENDED_ASCII_RANGE; ++i) {
        for (size_t j = 0; j < counts[i]; ++j, ++s) {
            *s = (char)i;
        }
    }
    
    return str;
}


/* *************************************************************************************************************************
 * strHasDups - String has duplicates - Determines whether string has duplicates or not
 * *************************************************************************************************************************/

inline bool strHasDups(char *str) {
    // struct array holds 256 single bit buckets. Initialize all bits to unset
    BitArray b_[EXTENDED_ASCII_RANGE] = {0};
    // Iterate through each character in the string and toggle the corresponding bit. If the bit has been toggled more than once, return true
    while (*str) {
        if (~(b_[*str++].bit ^= 1)) {
            return true;
        }
    }
    // Return false if no bits are toggled more than once (no duplicates exist)
    return false;
}


/* *************************************************************************************************************************
 * strCaseHasDups - String has duplicates - Case insensitive - Determines whether string has duplicates or not
 * *************************************************************************************************************************/

inline bool strCaseHasDups(char *str) {
    // struct array holds 256 single bit buckets. Initialize all bits to unset
    BitArray b_[EXTENDED_ASCII_RANGE] = {0};
    // Iterate through each character in the string and toggle the corresponding bit. If the bit has been toggled more than once, return true
    while (*str) {
        if (~(b_[lc(*str)].bit ^= 1)) {
            return true;
        } else ++str;
    }
    // Return false if no bits are toggled more than once (no duplicates exist)
    return false;
}

/* *************************************************************************************************************************
 * strPermutateAll - String permutate all - Generates all permutations of str and outputs them to the provided stream
 * *************************************************************************************************************************/

inline size_t strPermutateAll(char *str, FILE *stream) {
    
    // Return false if the stream or string is null, or if the string is empty
    if (!stream || !str || !*str) return false;
    
    // Save length of string, create array to hold permuted strings, define and initialize variable for permutation count
    size_t n, permutation_count = 0;
    char s[(n=strLen_(str)) + 1];
    
    // Determine if the string contains duplicates and call the corresponding permutation function. Return the functions return value
    return !strHasDups(str) ? _permutate_no_duplicates(strCpy_(s,str), n, stream, &permutation_count) : _permutate_has_duplicates(strCpy_(s,str), s, n, stream, &permutation_count);

}

/* =========================================================================================================================
 * _xor - XOR swap - Used by _permutate_no_duplicates
 * ========================================================================================================================*/

#define _xor(a,b) (a != b ? a ^= b, b ^= a, a ^= b : a)

/* =========================================================================================================================
 * _xor_iter - XOR iterator swap - Used by _permutate_has_duplicates
 * ========================================================================================================================*/

#define _xor_iter(a,b) (*a!=*b ? *a ^= *b, *b ^= *a, *a ^= *b : *a, a)

/* =========================================================================================================================
 * _min_element - Minimum element - Used by _permutate_has_duplicates
 * ========================================================================================================================*/

#define _min_element(a,b) ({                                                  \
    char *q = a;                                                              \
    for (char *p = q != b ? q+1 : b;   p != b;   q = *p < *q ? p : q, ++p);   \
    q;                                                                        \
})


/* *************************************************************************************************************************
 * _permutate_no_duplicates - Used by strPermutateAll - Permutation function for strings containing no duplicates. 
 * Returns the number of permutations generated.
 * *************************************************************************************************************************/

static inline size_t _permutate_no_duplicates(char *str, size_t n, FILE *stream, size_t *perm_count) {
    
    // Permutate the string recursively by swapping the first and n-1 th element, passing n-1 as the new string length each call
    if (n != 1) {
        for (char *p = str; p < str + n; ++p) {
            _xor(*p,str[n-1]), _permutate_no_duplicates(str,n-1,stream,perm_count), _xor(*p,str[n-1]);
        }
    } else { // Print the permutations once they've all been generated (n = 1) and increment permutation for each one
        fprintf(stream,"%s\n",str);
        ++*perm_count;
    }
    
    // Return the number of permutations generated for the given string
    return *perm_count;
}

/* *************************************************************************************************************************
 * _permutate_has_duplicates - Used by strPermutateAll - Permutation function for strings containing duplicates.
 * Returns the number of permutations generated.
 * *************************************************************************************************************************/

static inline size_t _permutate_has_duplicates(char *str, char *start, size_t n, FILE *stream, size_t *perm_count) {
    
    // Print the permutations to the provided stream and count each permutation by incrementing permutation count
    if (n == 0) {
        fprintf(stream,"%s\n",start);
        ++*perm_count;
        return 0;
    }
    
    // Find the minimum element within the given range: min ∈ [str, str + n]
    char *min = _min_element(str,str+n), *next_min, *p;
    
    // Continue permutating recursively: swap 1st element & new minimum element, subtract 1 from length (n)
    _permutate_has_duplicates(_xor_iter(str,min)+1, start, n-1, stream, perm_count);
    
    // Find the minimum element that is greater than the 1st element: next_min ∈ [str + 1, str + n); next_min > str && next_min ≤ str + i  ∀i ∈ n
    for (p = str+1, next_min = str+n; p < str + n; next_min = *p > *str && (next_min == str + n || *p < *next_min) ? p : next_min, ++p);
    
    // Loop continues iterating while a next_min character can be found
    while (next_min != str + n) {

        // Continue permutating recursively: swap 1st element & new minimum element, subtract 1 from length (n)
        _permutate_has_duplicates(_xor_iter(str,next_min)+1, start, n-1, stream, perm_count);
        
        // Attempt to find the minimum element that is greater than the 1st element: next_min ∈ [str + 1, str + n), ∀i ∈ n, (next_min > str && next_min ≤ str + i)
        for (p = str+1, next_min = str+n; p < str + n; next_min = *p > *str && (next_min == str + n || *p < *next_min) ? p : next_min, ++p);
    }
    
    // Return the number of permutations generated for the given string
    return *perm_count;
}

/* *************************************************************************************************************************
 * strCharCounts - String character counts - Obtains counts for occurences of each character in str and outputs them to
 * the provided stream. Output format => CHAR: COUNT
 * *************************************************************************************************************************/

inline bool strCharCounts(char *str, FILE *stream) {
    size_t counts[EXTENDED_ASCII_RANGE] = {0};
    bool ret;
    
    for (ret = str && *str ? true : false; ret && *str; ) {
        ++counts[*str++];
    }
    
    for (unsigned int i = 0; ret && i < EXTENDED_ASCII_RANGE; ++i) {
        if (counts[i]) fprintf(stream,"%c: %lu\n",(char)i,counts[i]);
    }
    return ret;
}

/* *************************************************************************************************************************/
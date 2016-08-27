/* ======================================================================================================================= *
 * Dave Dorzback                                                                                                           *
 * main.c                                                                                                                  *
 *                                                                                                                         *
 * Accepts input via command line arguments, sorts the arguments, then outputs them in order.                              *
 * Arguments are sorted using LSD radix sort.                                                                              *
 * Usage: ./program arg1 arg2 arg3 ,..., argN                                                                              *
 * ======================================================================================================================= */

/* *********************************************** INCLUDES/DECLARATIONS ***************************************************/
/*** Includes ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*** Macros ***/
/* Macros for color escape sequences */
#if !COLOR_OFF                  // Set COLOR_OFF true to remove color from output
    #define DEFAULT "\033[0m"   // Default color
    #define MAG "\x1b[35m"      // Magenta
    #define RED "\x1b[31m"      // Red
#else
    #define DEFAULT ""
    #define MAG ""
    #define RED ""
#endif

/* Macro for maximum word length */
#define MAX_WORD_LEN 20

/* Macros for radix sort - Used by sortWordList function */
#define R 27                                        // Number of buckets- 26 letters + 1 for null/other chars
#define islower(c) (c >= 'a' && c <= 'z' ? 1 : 0)   // Expression true if lowercase letter
#define isupper(c) (c >= 'A' && c <= 'Z' ? 1 : 0)   // Expression true if uppercase letter

/*** Prototypes ***/
int reallocWordList(char ***words, unsigned int n);
void freeWordList(char **words, unsigned int n);
void insertWord(char **words, char *word_n, int n);
void sortWordList(char **words, unsigned int n);    // Not used

/*** Utility Functions ***/
int isWord(char *str) { return (!*str) ? 1 : (!isalpha(*str)) ? 0 : isWord(++str); }   // Returns 1 if only letters in string, 0 if not
void clearStdin(void) { while (getchar()!='\n'); }                                     // Clears input from stdin buffer
/* *************************************************************************************************************************/

/* ******************************************************** MAIN ***********************************************************/
int main(void) {
    
    // Define variables and allocate char* array. Starts at an arbitrary size (25) and space is reallocated as needed
    unsigned int words_allocd = 25, n = 0;
    char **words = malloc(sizeof(*words) * words_allocd);
    
    // Get words from user
    while (1) {
        char input_string[MAX_WORD_LEN+1];
        uint8_t word_len;
        
        printf(MAG"Enter word: ");                      // Prompt user for word and get input using fgets
        fgets(input_string,MAX_WORD_LEN,stdin);
        
        if (input_string[(word_len=((uint8_t)strlen(input_string)))-1]!='\n') clearStdin(); // If input string too long, clear extra input from stdin
        else input_string[word_len-1] = '\0';                                               // Otherwise, change ending '\n' char to '\0'
        
        if (!*input_string) {                                                               // If no input entered, break loop
            break;
        } else if (!isWord(input_string)) {                                                 // If input not a word, print warning message and continue
            printf(RED"Warning: '%s' not accepted - letters only.\n",input_string);
            continue;                                                                       // Otherwise, realloc pointer array if needed and malloc space for word
        } else if ( (n==words_allocd && !reallocWordList(&words,words_allocd*=2)) || !(words[n] = malloc(word_len)) ) {
            printf(RED"Error: memory allocation failed.\n");
            break;
        } else insertWord(words,input_string,n++);      // Save word to its proper position using insertWord, increment n
    }                                                   // Using strcpy(words[n++],input_string) and calling sortWordList(words,n) outside loop would produce the same result

    // If no words were saved to word_list, free used memory, print error & exit. If there's unused space, realloc to size n
    if (!n) freeWordList(words,n), printf(RED"Error: No words entered.\n"MAG"\nNow Exiting...\n"DEFAULT), exit(0);
    else if (words_allocd!=n) reallocWordList(&words,n);
    
    // Print the words in sorted order
    printf(MAG"\nIn sorted order: ");
    for (int i = 0; i < n; i++) {
        printf("%s%c",words[i],(i!=n-1) ? ' ' : '\n');
    }
    
    // Free memory used by unsorted word array
    freeWordList(words, n);
    
    // Restore console color and exit
    printf("\nNow Exiting...\n"DEFAULT);
    return 0;
}
/* ****************************************************** END MAIN *********************************************************/

/* ***************************************************** FUNCTIONS *********************************************************/
/* **************************************************************************************************************************
 * insertWord - Accepts pointer to char* array (array holding words), a pointer to the word to be inserted, and an int for  *
 * the size of the array. Copies new word to end (n) of array, then starts at element n-1 and compares each word to the new *
 * word until a word of lower/equal value is found; or the front of the array (0th position) is reached. Each pointer in    *
 * the array is then shifted back 1 position until the insertion index (idx) is reached. Pointer to the new word is then    *
 * saved to its proper position in the array.                                                                               *
 * **************************************************************************************************************************/
void insertWord(char **words, char *word_n, int n) {
    strcpy(words[n],word_n);                               // Copy new word at end
    for (int i = n-1, idx; i >= 0; i--) {                  // Compare new word to each word until lower/equal word or 0th pos
        if ((idx = (strcmp(word_n,words[i])>=0) ? i+1 : (!i) ? 0 : -1) >= 0) {
            word_n = words[n];                             // Save the address of new word (stored at end of array) to word_n
            while ((words[n] = words[n-1]) && n-- > idx);  // Shift each pointer in the array back 1 position up to idx
            words[idx] = word_n;                           // Save pointer to new word to its proper position in array
            return;
        }
    }
}
/* **************************************************************************************************************************
 * reallocWordList - Accepts pointer to array of pointers to chars and int for number of pointers to allocate memory for.   *
 * Reallocates the char* array, 'words', to new size, n. If successful, saves address of new memory to words and returns 1. *
 * If unsuccessful, 'words' is unchanged and 0 is returned.                                                                 *
 * **************************************************************************************************************************/
int reallocWordList(char ***words, unsigned int n) {
    char **new_words = realloc(*words, sizeof(**words) * n);    // realloc 'words' and save new address
    if (new_words) {                                            // If realloc was successful: save new address to 'words', return 1
        *words = new_words;
        return 1;
    } else return 0;                                            // If realloc failed, return 0
}
/* **************************************************************************************************************************
 * freeWordList - Accepts char* array and integer for number of elements. Frees the array's memory.                         *
 * **************************************************************************************************************************/
void freeWordList(char **words, unsigned int n) {
    for (int i = 0; i < n; i++) {
        free(words[i]);                 // Free each row
    }
    free(words);                        // Free the array of row pointers
}

/* Note: This function isn't used because the words are kept in order using insertWord function. But this works to sort the
 words and is the method I used originally so I decided to leave it here. */
/* **************************************************************************************************************************
 * sortWordList - Accepts char* array, int for array size and int for length of longest word in array.                      *
 * Sorts the words using LSD radix sort. Assumes strings contain only letters (other values given 0 rank) and ignores case  *
 * (words of either case given equal rank). Ascii values are adjusted so a-z/A-z => 1-26. Null chars/all other values => 0. *
 * **************************************************************************************************************************/
void sortWordList(char **words, unsigned int n) {
    int max_word_len = (int)strlen(words[0]);
    for (int i = 1; i < n; i++) {                                   // Get the maximum word length
        if (strlen(words[i])>max_word_len) max_word_len = (int)strlen(words[i]);
    }
    for (int idx = max_word_len-1; idx >= 0; idx--) {               // Start at right most character of longest word
        int b_[R] = {0};
        char *tmp[n];                                               // Temp word array
        for (int i = 0; i < n; i++) {                               // Increment given bucket for each occurrence
            ++b_[(islower(words[i][idx])) ? words[i][idx] - 96 : (isupper(words[i][idx])) ? words[i][idx] - 64 : 0];
        }
        for (int i = 1; i < R; i++) {                               // Calculate starting index for each bucket
            b_[i] += b_[i-1];
        }
        for (int i = n - 1; i >= 0; i--) {                          // Save ordered strings to tmp
            tmp[--b_[(islower(words[i][idx])) ? words[i][idx] - 96 : (isupper(words[i][idx])) ? words[i][idx] - 64 : 0]] = words[i];
        }
        for (int i = 0; i < n; i++) {                               // Save strings in sorted order to words
            words[i] = tmp[i];
        }
    }
}
/* **************************************************************************************************************************/
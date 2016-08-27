/* ======================================================================================================================= *
 * Dave Dorzback                                                                                                           *
 * textfile.h                                                                                                              *
 * ======================================================================================================================= */

#ifndef textfile_h
#define textfile_h

/* **************************************************** MACROS *************************************************************/
#define DEFAULT "\033[0m"
#define RED "\x1b[31m"
#define BLUE "\x1b[34m"
#define KCYN "\x1B[36m"
#define MAX_WORD_LEN 60
/* *************************************************** TYPEDEFS ************************************************************/
typedef struct {                        /* Struct for holding information about text file */
    char *filepath;                     // Filepath
    unsigned long int word_count;       // Word count
    unsigned long int line_count;       // Line count
    unsigned long int emptyl_count;     // Empty line count
    unsigned long int char_count;       // Character count
    unsigned long int white_count;      // Whitespace count (spaces, tabs, newlines)
    unsigned short int f_accessed;      // Indicates whether the file at filepath has been accessed or not
} txtFileInfo;

typedef struct node {                   /* Struct for creating linked list to store bag of words data */
    char *word;                         // String to store distinct word
    unsigned int word_count;            // Int to store number of occurrences of word
    struct node *next;                  // Struct pointer store address of the next node
} WordList;
/* ************************************************** PROTOTYPES ***********************************************************/
extern void getFileData(txtFileInfo *file, const char *filepath);

extern void printFileData(txtFileInfo *file);

extern WordList *getBagofWords(WordList *bagofwords, const char *input_fp);

extern void writeBagofWords(WordList **bagofwords, const char *output_fp);

static inline WordList *insertNode(WordList *root, char *word);
/* *************************************************************************************************************************/

#endif /* textfile_h */
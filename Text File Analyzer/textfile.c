/* ======================================================================================================================= *
 * Dave Dorzback                                                                                                           *
 * textfile.c                                                                                                              *
 * ======================================================================================================================= */

/* *************************************************** INCLUDES *************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "textfile.h"

/* *************************************************** FUNCTIONS ************************************************************/
/* **************************************************************************************************************************
 * getFileData - Stores the file path and counts for chars/words/spaces etc. to txtFileInfo parameter.                      *
 * Words, spaces, empty lines and non-empty lines are counted using a simple finite state machine implementation.           *
 * **************************************************************************************************************************/
extern void getFileData(txtFileInfo *file, const char *filepath) {
    
    // Set the file path and initialize the counters
    file->filepath = (char*)filepath;
    file->word_count = file->line_count = file->emptyl_count = file->char_count = file->white_count = file->f_accessed = 0;
    
    // Open the text file
    FILE *txtfile = fopen(file->filepath, "r");
    if (!txtfile) { // Print error and return if the file was not opened successfully
        printf(RED"Error: could not open input file. Check file path and format.\n"DEFAULT);
        return;
    } else file->f_accessed = 1;
    
    // enum 'states' for possible states of file input and define variables
    enum {INITIAL, SPACE, LINE, WORD, FINAL};
    char c, state = INITIAL, word[MAX_WORD_LEN+1];
    
    // Loop runs until state = FINAL, set when EOF reached
    while (state != FINAL) {
        // Get new c each time. If c is not whitespace, unget from stream and use fscan to read entire word (reduces iterations)
        if (!isspace(c = getc(txtfile))) ungetc(c,txtfile), fscanf(txtfile,"%s",word);
        
        // Switch statement used to create a simple finite-state machine for counting different occurrences within the text file
        switch (state) {
            case INITIAL:           // c <=> Initial state
                if (isspace(c))
                    if (c == '\n') ++file->white_count, ++file->emptyl_count, state = LINE;
                    else ++file->white_count, state = SPACE;
                else if (c != EOF) ++file->word_count, state = WORD;
                else state = FINAL;
            break;
            case SPACE:             // c <=> Whitespace state (' ' or '\t')
                if (isspace(c))
                    if (c == '\n') ++file->white_count, ++file->line_count, state = LINE;
                    else ++file->white_count, state = SPACE;
                else if (c != EOF) ++file->word_count, state = WORD;
                else ++file->line_count, state = FINAL;
            break;
            case LINE:              // c <=> Newline state ('\n')
                if (isspace(c))
                    if (c == '\n') ++file->white_count, ++file->emptyl_count, state = LINE;
                    else ++file->white_count, state = SPACE;
                else if (c != EOF) ++file->word_count, state = WORD;
                else state = FINAL;
            break;
            case WORD:              // c <=> Word state (all other non-whitespace chars)
                if (isspace(c))
                    if (c == '\n') ++file->white_count, ++file->line_count, state = LINE;
                    else ++file->white_count, state = SPACE;
                else if (c == EOF) ++file->line_count, state = FINAL;
            break;
        }
    }
    
    // Get the number of non-whitespace characters by subtracting the white_count from total number of characters
    // Note that this calculation may not be accurate if the file contains non-standard single byte characters
    file->char_count = ftello(txtfile) - file->white_count;
    
    // Close the text file
    fclose(txtfile);
}

/* **************************************************************************************************************************
 * printFileData - Accepts pointer to txtFileInfo struct and prints the data stored in it.                                  *
 * **************************************************************************************************************************/
extern void printFileData(txtFileInfo *file) {
    // Check to see if file has been accessed, display 'no data' message if not
    if (!file || !file->f_accessed) {
        printf(KCYN"No file data to display. File has not been accessed.\n"DEFAULT);
        return;
    } // Otherwise, display count data for the file
    printf(KCYN"File: %s\n",file->filepath);                                        // Print file path
    printf("Number of characters: %7lu\n",file->char_count+file->white_count);      // Print total char count
    printf(BLUE"  -Whitespace: %14lu\n",file->white_count);                         // Print whitespace count
    printf("  -Non-whitespace: %10lu\n",file->char_count);                          // Print non-whitespace count
    printf(KCYN"Number of words: %12lu\n",file->word_count);                        // Print word count
    printf("Number of lines: %12lu\n",file->line_count+file->emptyl_count);         // Print total line count
    printf(BLUE"  -Empty lines: %13lu\n",file->emptyl_count);                       // Print empty line count
    printf("  -Non-empty lines: %9lu\n"DEFAULT,file->line_count);                   // Print non-empty line count
}

/* **************************************************************************************************************************
 * strip - used by getBagofWords - Removes all punctuation (non-alpha chars) and capitalization from 'word'. Using a macro  *
 * for this instead of a function increases speed of getBagofWords considerably.                                            *
 * **************************************************************************************************************************/
#define strip(word) do {                                \
    for (char *p = word; *p;) {                         \
        if (!isalpha(*p = tolower(*p))) {               \
            memcpy(p,p+1,strlen(p));                    \
        } else ++p;                                     \
    }                                                   \
} while (0)

/* **************************************************************************************************************************
 * getbagofWords - Accepts pointer to WordList struct and string for file path (to .txt file) and creates/expands a linked  *
 * list containing each word and its count. If a NULL value is used as the first argument, a new linked list is created.    *
 * Otherwise, the list specified in the 1st argument is expanded/updated using data from the new txt file and a pointer to  *
 * the root node is returned upon completion. This allows for creating a single bag of words with data from multiple files. *                                *
 * Note: For large & very large files/data sets, this method of inserting words into an ordered linked list will likely     *
 * have high memory requirements and may not be efficient enough to work well, using a hashmap or trie would probably give  *
 * better results. For small files/amounts of data, however, this method should be sufficient.                              *
 * **************************************************************************************************************************/
extern WordList *getBagofWords(WordList *bagofwords, const char *input_fp) {
    
    FILE *txtfile = fopen((char *)input_fp, "r");        // Open the text file
    
    if (!txtfile) {                                      // Print error and return if file cannot be opened
        printf(RED"Error: could not open bag of words input file. Check file path and format.\n"DEFAULT);
        return NULL;
    } else printf(KCYN"Processing input file: '%s'. . .\n"DEFAULT,input_fp);
    
    char c;
    
    while ((c = getc(txtfile)) != EOF) {                        // Loop until EOF and look for words (non-whitespace characters)
        if (!isspace(c)) {
            char word[MAX_WORD_LEN+1] = {'\0'};
            ungetc(c,txtfile), fscanf(txtfile,"%s",word);       // If c is not a space, unget c from stream and fscanf next word
            strip(word);                                        // Remove punctuation and capitalization from the word
            bagofwords = insertNode(bagofwords, word);          // Add the word to the list/increment the word's counter
        }
    }
    
    fclose(txtfile);                                            // Close the file and return the root node
    return bagofwords;
}

/* **************************************************************************************************************************
 * writeBagofWords - Accepts a pointer to WordList pointer (pointer to linked list root node) and writes each entry's word/ *
 * word count to bagofwords.txt in descending order of their word count. Each node is removed from the list and free'd after*
 * its entries have been written to the file. Prints error message if output file cannot be opened or errors encountered    *
 * while writing output, otherwise prints success message.                                                                  *
 * Note: Function will always free the memory used by the argument its passed (**bagofwords), meaning the argument list     *
 * should not be accessed after execution.                                                                                  *
 * **************************************************************************************************************************/
extern void writeBagofWords(WordList **bagofwords, const char *output_fp) {
    
    // Open the text file for writing
    int fpf, fpf_error = 1, max = 0;
    FILE *txtfile = fopen(output_fp,"w+");
    
    // If the file cannot be opened or the parameter is NULL, free the lists memory (when necessary) & print error
    if (!txtfile || !*bagofwords) {
        for (WordList *node = *bagofwords; node!=NULL; node = node->next) {
            free(node->word);
            free(node);
        }
        printf(RED"Error: '%s' could not be opened. Check file path and format.\n"DEFAULT,output_fp);
        return;
    }
    
    // Find the word with the highest word count and save that value to max
    for (WordList *node = *bagofwords; node!=NULL; node = node->next) {
        if (node->word_count > max) max = node->word_count;
    }
    
    // Outer loop iterates from max - 1. Inner loop starts at root node and compare's each word count to i.
    // When a match is found, the node's word/word count are written to the file, then the node is removed from the list.
    for (int i = max; i >= 1; i--) {
        for (WordList **node = bagofwords, *leading_node; *node!=NULL;) {
            if ((*node)->word_count == i) {
                fpf_error = ((fpf=  fprintf(txtfile, "%s: %d\n",(*node)->word, (*node)->word_count)) < 0 && fpf_error >= 0) ? fpf : 0;
                leading_node = (*node)->next;                                   // Get node proceeding the current node
                free((*node)->word), free(*node);                               // Free the current node
                *node = leading_node;                                           // Link the list back together
            } else node = &(*node)->next;                                       // If word count doesn't equal i, move to next node
        }
    }
    
    // Close file, print error message if any writes unsuccessful otherwise print success message
    fclose(txtfile);
    printf("%s '%s'.\n"DEFAULT,fpf_error<0 ? RED"Error: unsuccessful bag of words write to" : KCYN"Successfully wrote bag of words to",output_fp);
}

/* **************************************************************************************************************************
 * insertNode - Accepts pointer to WordList and a string. Finds the proper position to insert a node for the given string   *
 * and either inserts a new node (if the given word does not appear in the list already) or increments the word count for   *
 * the given string. The list is kept in order based on the value of each node's 'word' member. They are ordered in an      *
 * alphabetical manner, with the root node having the highest alpha value for its 'word' member. This eliminates the need   *
 * to transverse/search the entire list every time a new word is added; instead only the part of the list containing words  *
 * of lower values needs to be transversed/searched each time.                                                              *
 * **************************************************************************************************************************/
static inline WordList *insertNode(WordList *root, char *word) {
    
    // Make sure word is not an empty string, return if so
    if (!*word) return root;
    
    // Define new node variable and variables for 'leading' and 'preceding' nodes
    WordList *new_node, *l, *p;
    
    // Find the position in the list to insert the new word (nodes kept in order based on alpha value of their word)
    for (l = root, p = NULL; l && strcmp(word,l->word) < 0; p = l, l = l->next);
    
    // If the new word is not already in list, create a node for it and insert it in its proper position
    if (l == NULL || strcmp(word,l->word) != 0) {       // malloc space for node and string, print error/exit if failure
        if ((!(new_node = malloc(sizeof(WordList))) || !(new_node->word = malloc(strlen(word)+1))))
            printf(RED"Error: memory allocation failed.\n\n"KCYN"Now Exiting...\n"DEFAULT), exit(1);
        strcpy(new_node->word,word);                    // Copy word to new node and set count to 1
        new_node->word_count = 1;
        new_node->next = l;                             // Link the leading node to the new node
        p ? (p->next = new_node) : (root = new_node);   // Link the preceding node to the new node (unless its the new root node)
    } else ++l->word_count;                             // Otherwise, increment the word count for the given word
    // 
    // Return the root node
    return root;
}

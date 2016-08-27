/* ======================================================================================================================= *
 * Dave Dorzback                                                                                                           *
 * main.c                                                                                                                  *
 *                                                                                                                         *
 * Text File Analyzer - Example use of textfile.h functions. Accepts a file path for a .txt file via command line argument *
 * and outputs counts for characters, whitespace, lines, empty lines, ect. to console. The same .txt file is then used to  *
 * create a bag of words, and the data is written to 'bagofwords.txt'.                                                     *
 * ======================================================================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include "textfile.h"

int main(int argc, const char *argv[]) {
    
    // Print error and exit if incorrect number of command line arguments
    if (argc!=2) {
        printf(RED"Error: incorrect argument count. Provide only a file path.\n"DEFAULT);
        exit(1);
    }
    
    // Define txtFileInfo struct and get/print information on the text file
    txtFileInfo wordfile;
    getFileData(&wordfile, argv[1]);
    printFileData(&wordfile);
    
    // Create the bag of words with the provided text file
    WordList *bagofwords = getBagofWords(NULL, argv[1]);
    
    // Write the bag of words to "bagofwords.txt"
    writeBagofWords(&bagofwords,"bagofwords.txt");
    
    // Print exit message
    printf(KCYN"\nNow Exiting...\n"DEFAULT);
    return 0;
}
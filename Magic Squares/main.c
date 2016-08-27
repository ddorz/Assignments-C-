/* =========================================================================================================================================================================*
 * Dave Dorzback                                                                                                                                                            *
 * main.c                                                                                                                                                                   *
 *                                                                                                                                                                          *
 * Program for creating and manipulating odd nxn magic squares.                                                                                                             *
 * =========================================================================================================================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include "MagicSquares.h"

int main(void) {
    
    /* Variables for magic square, magic square size and user input:  */
    int **magic_square, *n, option;
    
    /* Array of function pointers for magic square functions: */
    void (*menuOptions[15])() = {
        display_menu,               // 0  - Not an option user can select, but menuOptions[0]() used to display menu
        reverse_rows_ms,            // 1  - Reverse magic square rows
        reverse_columns_ms,         // 2  - Reverse magic square columns
        rotate_90_ms,               // 3  - Rotate magic square 90 degrees
        rotate_180_ms,              // 4  - Rotate magic square 180 degrees
        rotate_270_ms,              // 5  - Rotate magic square 270 degrees
        transpose_ms,               // 6  - Transpose magic square
        transpose_r_ms,             // 7  - Transpose magic square in reverse (reflect over secondary diagonal)
        increase_ms,                // 8  - Increase magic square values by input amount
        decrease_ms,                // 9  - Decrease magic square values by input amount
        random_increase_ms,         // 10 - Increase magic square values by random amount
        random_decrease_ms,         // 11 - Decrease magic square values by random amount
        max_increase_ms,            // 12 - Increase magic square values by max amount
        max_decrease_ms,            // 13 - Decrease magic square values by max amount
        reset_ms                    // 14 - Reset magic square to original
    };

    /* Prompt & get input for size of magic square. Verify input was valid: */
    printf(MAG);
    if ((n=get_size())==NULL) {
        printf("\nNow Exiting...\n"DEFAULT);
        exit(1);                                                            // Exit if input invalid
    }
        
    /* Create magic square & verify it was successful. Print error and exit if unsuccessful: */
    if (!(magic_square = create_ms(*n))) {
        printf(RED"\nError! Failed to allocate memory.\n"MAG"\nNow Exiting...\n"DEFAULT);
        exit(1);
    }
    
    /* Primary program loop - displays menu, gets user input menu option, executes corresponding function: */
    do {
        menuOptions[0]();
        printf(KCYN);
        print_ms(magic_square, *n);
        printf("Sum of Magic Numbers: %d\n", sum_ms(magic_square, *n));
        printf(MAG"Enter an option or any key to quit (0 - 14): ");
        if (!(scanf("%d",&option)) || getchar()!='\n' || option <= 0 || option > 14) {
            break;                                                          // Break if invalid input is entered
        }
        menuOptions[option](magic_square, *n);                              // Execute the function corresponding to user's input
    } while (1);
    
    /* Free memory & restore console color before exiting: */
    free_ms(magic_square, *n);
    printf("\nNow Exiting...\n"DEFAULT);
    return 0;
}
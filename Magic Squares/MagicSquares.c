/* =========================================================================================================================================================================*
 * Dave Dorzback                                                                                                                                                            *
 * MagicSquares.c                                                                                                                                                           *
 *--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*
 *                                                            Creating an Odd nxn Magic Square                                                                              *
 *--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*
 * The process for filling an odd nxn magic square is illustrated below. The general process is the same for any nxn magic square, where n is odd.                          *
 *                                                                                                                                                                          *
 * After placing the first 3 numbers, we find the 4th position is already occupied - (figure [3]).                                                                          *
 * So, we place the next number (4) one row below the previous position (where the number 3 is).                                                                            *
 * This position is down two rows and left one column ([r+2][c-1]) from the current position (X), and we place the next number (4) there - (figure [4]).                    *
 *          [1]                            [2]                             [3]                            [4]                                                               *
 * _____________________          _____________________            _____________________          _____________________                                                     *
 * |     |   1  |      |          |     |   1  |      |            |     | 1(X) |      |          |     | 1(X) |      |                                                     *
 * |-----|------|------|          |-----|------|------|            |-----|------|------|          |-----|------|------|                                                     *
 * |     |      |      |   ==>    |  X  |      |      |    ==>     |  3  |      |      |   ==>    |  3  |      |      |                                                     *
 * |-----|------|------|          |-----|------|------|            |-----|------|------|          |-----|------|------|                                                     *
 * |     |      |   X  |          |     |      |  2   |            |     |      |  2   |          |  4  |      |  2   |                                                     *
 * ---------------------          ---------------------            ---------------------          ---------------------                                                     *
 * We need not change our row and column variables (our position) to put the number 4 in place, so our position (X) remains the same (where the number 1 is).               *
 * Moving to the next position requires we move up one row and right one column from where the last number was placed (in this case, the number 4).                         *
 * Or, equivalently, one row below the current position ([r+1][c]) - (figure [5]).                                                                                          *
 *         [5]                                 [6]                                                                                                                          *
 * _____________________              _____________________                                                                                                                 *
 * |     |   1  |      |              |     |   1  |   X  |                                                                                                                 *
 * |-----|------|------|              |-----|------|------|                                                                                                                 *
 * |  3  |   X  |      |     ==>      |  3  |   5  |      |     ==>    . . .                                                                                                *
 * |-----|------|------|              |-----|------|------|                                                                                                                 *
 * |  4  |      |  2   |              |  4  |      |  2   |                                                                                                                 *
 * ---------------------              ---------------------                                                                                                                 *
 * The next position is unoccupied, so we place the next number there and continue the process - (figure [6]).                                                              *
 *--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*
 *                                                                                                                                                                          *
 * =========================================================================================================================================================================*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Includes: */

#include "MagicSquares.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Constants: */

static const int MAX_MAGIC_NUMBER = 30000;      // Maximum value allowed in given magic square (somewhat arbitrary, higher values will work)
static const int MIN_MAGIC_NUMBER = 1;          // Minimum value allowed in given magic square
static const int MAX_RAND = 10;                 // Upper limit for range of randomly generated values (for increasing/decreasing magic numbers)
static const int MIN_RAND = 1;                  // Lower limit for range of randomly generated values (for increasing/decreasing magic numbers)
static const int MAX_SIZE = 11;                 // Maximum size allowed for creating magic square (somewhat arbitrary, works with larger sizes)
static const int MIN_SIZE = 1;                  // Minimum size allowed for creating magic square

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Utility Functions: */

/* Function for clearing stdin */
static inline void clear_stdin(void) { while(getchar()!='\n'); }

/* Functions for determing the current and original center numbers of nxn magic square */
static inline int get_original_center( int n)          { return n*(n/2) + ((n/2)+1); }
static inline  int get_current_center(int **ms, int n)  { return ms[n/2][n/2]; }

/* Functions for determing the current and original max numbers of nxn magic square */
static inline int get_original_largest(int n)          { return n*n; }
static inline int get_current_largest(int **ms, int n) { return (get_original_largest(n) + (get_current_center(ms,n) - get_original_center(n))); }

/* Functions for determing the maximum amounts magic numbers can be increased/decreased */
static inline int get_max_increase(int **ms, int n)    { return MAX_MAGIC_NUMBER - get_current_largest(ms,n); }
static inline int get_max_decrease(int **ms, int n)    { return get_current_center(ms,n) - get_original_center(n); }

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Function Definitions: */

/* **************************************************************************************************************************************************************************
 * Get Magic Square Size - Gets the size of magic square's side from user and returns pointer to size variable. NULL return indicates error/invalid input.                  *
 * *************************************************************************************************************************************************************************/
int *get_size(void) {
    int *size = alloc(1,*size);
    printf("Enter size of magic square: ");                              // Prompt user, input size, validate input
    if (!(scanf("%d",size)) || getchar()!='\n') {
        clear_stdin();
        printf(RED"Invalid Input: Non-integer value\n"DEFAULT);                 // If non-integer input: clear stdin, print error & return null
        return NULL;
    } else if ((*size < MIN_SIZE) || even(*size) || (*size > MAX_SIZE)) {   // Verify size is within set range (1-11) and is an odd integer
        printf(RED"Invalid Input: %d\n"DEFAULT,*size);                          // If input is outside range or is even: print error & return null
        return NULL;
    } else {
        return size;                                                        // Otherwise, input is valid: return size pointer
    }
}

/* **************************************************************************************************************************************************************************
 * Create Magic Square - Allocate pointer so it can be indexed as an nxn array & fill in magic numbers.                                                                    *
 * Filling Magic Square: If element unoccupied, save value (i) there, then move to next position (one row down and one column right).                                       *
 * Otherwise, element occupied, save value (i) one row below the previous position, then move to next position (which will be one row below the current position).          *
 * Note: See EOF for more info on filling the magic square.                                                                                                                 *
 * *************************************************************************************************************************************************************************/
int **create_ms(int n) {
    /* Allocate array and return null if any calloc calls fail */
    int **ms = alloc(n,*ms);            // allocate array of n pointers & save address
    if (!ms) {
        return NULL;
    } else {
        for (int i = 0; i < n; i++) {   // allocate space for each row's buckets and save addresses
            ms[i] = alloc(n,*ms[i]);
            if (!ms[i]) {
                free_ms(ms,i);          // free used memory & return null if any calloc call fails
                return NULL;
            }
        }
    }
    /* Fill magic square- 1st position: ms[0][n/2]. */
    /* To move up 1 row (or 1 column right): decrement by 1 (or add n-1). Modulus used for controlling wrap arounds. */
    /* To move 1 column left (or 1 row down): increment by 1. Modulus used for controlling wrap arounds. */
    for (int i = 1, r = 0, c = n/2; i <= n*n; i++) { /* If occupied: save value, adjust r, adjust c. Else: save value (2 rows down & 1 column left), adjust r. */
        (!ms[r][c]) ? (ms[r][c] = i, r = (r+n-1)%n, c = (c+1)%n) : (ms[(r+2)%n][(c+n-1)%n] = i, r = (r+1)%n);
    }
    return ms;
}

/* **************************************************************************************************************************************************************************
 * Frees Magic Square - Frees magic square memory (in the opposite order it was allocated).                                                                                 *
 * *************************************************************************************************************************************************************************/
void free_ms(int **ms, int n) {
    for (int i = 0; i < n; i++) {
        free(ms[i]);                    // free each row first
    }
    free(ms);                           // then free array of row pointers
}

/* **************************************************************************************************************************************************************************
 * Print Magic Square - Prints the magic square. (Magic square printed in cyan)                                                                                             *
 * *************************************************************************************************************************************************************************/
void print_ms(int **ms,int n) {
    for (int i = 0; i < n*n; i++) {
        printf("%6.0d%s",ms[i/n][i%n],(i%n==n-1) ? "\n" : "");  // print newline when element is last in its row (when i%n=n-1)
    }
}

/* **************************************************************************************************************************************************************************
 * Sum Magic Numbers - Sums all the magic square's numbers using explicit formula for sum of consecutive integers n(n+1)/2 where n = n^2. Since magic numbers may have been *
 * increased or changed position, the original value of center number (which never moves) for the nxn magic square is calculated using the formula:                         *
 * center = n*(n/2) + ((n/2)+1). The difference between the current center value and original center value is then found and multiplied by n^2, to account for any increase *
 * in the values. The final formula for the sum is: sum = (n^2*(n^2+1) / 2) + ( (ms[n/2][n/2] - (n * floor(n/2) + ceiling(n/2)) ) * n^2).                                   *
 * *************************************************************************************************************************************************************************/
int sum_ms(int **ms, int n) {
    return ((n*n*((n*n)+1))/2) + ((ms[n/2][n/2] - (n*(n/2) + ((n/2)+1)))*(n*n));
}

/* **************************************************************************************************************************************************************************
 * Reverse Rows of Magic Square - Reverses each row of the nxn magic square array to create a new magic square.                                                             *
 * *************************************************************************************************************************************************************************/
void reverse_rows_ms(int **ms, int n) {
    for (int i = 0; i < n; i++) {           // outer loop runs n iterations (one for each row)
        for (int j = 0; j < (n/2); j++) {   // inner loop runs n/2 iterations for each row. n/2 swaps take place within each row
            swap(ms[i][j],ms[i][n-j-1]);    // [0][0] <-swap-> [0][n-1], [0][1] <-swap-> [0][n-2],...etc.
        }
    }
}

/* **************************************************************************************************************************************************************************
 * Reverse Columns of Magic Square - Reverses each column of the nxn magic square array to create a new magic square.                                                       *
 * *************************************************************************************************************************************************************************/
void reverse_columns_ms(int **ms, int n) {
    for (int i = 0; i < n; i++) {           // out loop runs n iterations (one for each column)
        for (int j = 0; j < (n/2); j++) {   // inner loop runs n/2 iterations for each column. n/2 swaps take place within each col
            swap(ms[j][i],ms[n-j-1][i]);    // [0][0] <-swap-> [n-1][0], [1][0] <-swap-> [n-2][0],...etc.
        }
    }
}

/* **************************************************************************************************************************************************************************
 * Rotate Magic Square 90 Degrees - Rotates the magic square 90 degrees to create a new magic square.                                                                       *
 * *************************************************************************************************************************************************************************/
void rotate_90_ms(int **ms, int n) {
    /* Transpose nxn magic square array */
    for (int i = 0; i < n; i++) {           // outer loop runs n iterations
        for (int j = i+1; j < n; j++) {     // inner loop runs n-1 iterations, then n-2, then n-3,...,then n-a, etc. (where n-a > 0).
            swap(ms[i][j],ms[j][i]);        // [0][1] <-swap-> [1][0], [0][2] <-swap-> [2][0],...etc.
        }
    }
    /* Reverse each row */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (n/2); j++) {
            swap(ms[i][j],ms[i][n-j-1]);
        }
    }
}

/* **************************************************************************************************************************************************************************
 * Rotate Magic Square 180 Degrees - Rotates the magic square 180 degrees (reverses it) to create a new magic square.                                                       *
 * *************************************************************************************************************************************************************************/
void rotate_180_ms(int **ms, int n) {
    /* Reverse each row by swapping the corresponding elements */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (n/2); j++) {
            swap(ms[i][j],ms[i][n-j-1]);
        }
    }
    /* Reverse each column by swapping the corresponding elements */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (n/2); j++) {
            swap(ms[j][i],ms[n-j-1][i]);
        }
    }
}

/* **************************************************************************************************************************************************************************
 * Rotate Magic Square 270 Degrees - Rotates the magic square 270 degrees to create a new magic square.                                                                     *
 * *************************************************************************************************************************************************************************/
void rotate_270_ms(int **ms, int n) {
    /* Transpose nxn magic square array */
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            swap(ms[i][j],ms[j][i]);
        }
    }
    /* Reverse each column */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (n/2); j++) {
            swap(ms[j][i],ms[n-j-1][i]);
        }
    }
}

/* **************************************************************************************************************************************************************************
 * Transpose Magic Square - Transposes (reflect over primary diagonal) the magic square to create a new magic square.                                                       *
 * *************************************************************************************************************************************************************************/
void transpose_ms(int **ms, int n) {
    /* Transpose the nxn magic square array */
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            swap(ms[i][j],ms[j][i]);
        }
    }
}

/* **************************************************************************************************************************************************************************
 * Reverse Transpose Magic Square - Transposes the magic square backwards (reflect over secondary diagonal) to create a new magic square.                                  *
 * *************************************************************************************************************************************************************************/
void transpose_r_ms(int **ms, int n) {
    /* Reverse each row */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (n/2); j++) {
            swap(ms[i][j],ms[i][n-j-1]);
        }
    }
    /* Reverse each column */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (n/2); j++) {
            swap(ms[j][i],ms[n-j-1][i]);
        }
    }
    /* Transpose nxn magic square array */
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            swap(ms[i][j],ms[j][i]);
        }
    }
}

/* **************************************************************************************************************************************************************************
 * Increase Magic Square - Increases all numbers in the magic square by a given amount to create a new magic square.                                                        *
 * Find the largest number in the magic square (current_max) to make sure the increase won't cause any numbers to exceed the upper limit (MAX_MAGIC_NUMBER).                *
 * Since we don't know if the magic square has been transformed or not, uses the center number (which never moves) to determine the largest number in the magic square.     *
 * *************************************************************************************************************************************************************************/
void increase_ms(int **ms, int n) {
    int current_max = get_current_largest(ms,n), amount;
    if (current_max != MAX_MAGIC_NUMBER) {                                      // Make sure magic numbers are below max values, display error if not
        printf("Enter amount to increase magic square values by: ");            // Prompt user for increase amount
        if ((scanf("%d",&amount)) && getchar()=='\n') {                         // Make sure the input was valid, display error & clear stdin if not
            if ((current_max+amount <= MAX_MAGIC_NUMBER) && (amount > 0)) {     // Make sure amount is valid (positive & keeps values below max), display error if not
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        ms[i][j] += amount;                                     // If all conditions pass, increase magic numbers by amount
                    }
                }
            } else printf(RED"\nError! Invalid amount (positive amounts only). Max magic number limit: %d\n"DEFAULT,MAX_MAGIC_NUMBER);
        } else printf(RED"\nError! Invalid input.\n"DEFAULT), clear_stdin();
    } else printf(RED"\nError! Values at maximum. Maximum allowed magic number: %d\n"DEFAULT,MAX_MAGIC_NUMBER);
}

/* **************************************************************************************************************************************************************************
 * Decrease Magic Square -  Decreases all numbers in the magic square by a given amount to create a new magic square (magic square needs to have been increased).           *
 * Makes sure the decrease amount won't cause any of the magic numbers to go below the lower limit, 1 (MIN_MAGIC NUMBER).                                                   *
 * Since we don't know if the magic square has been transformed or not, uses the center number (which never moves) to verify this.                                          *
 * *************************************************************************************************************************************************************************/
void decrease_ms(int **ms, int n) {
    int default_center = get_original_center(n), current_center = get_current_center(ms,n), amount;
    if (current_center != default_center) {                                     // Make sure magic numbers are above min (original) values, display error if not
        printf("Enter amount to decrease magic square values by: ");            // Prompt user for decrease amount
        if ((scanf("%d",&amount)) && getchar()=='\n') {                         // Make sure input is valid, display error & clear stdin if not
            if ((current_center-amount > default_center) && (amount > 0)) {     // Make sure amount is valid (positive & keeps values above min), display error if not
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        ms[i][j] -= amount;                                     // If all conditions pass, decrease magic numbers by amount
                    }
                }
            } else printf(RED"\nError! Invalid amount (positive amounts only). Min magic number limit: %d\n"DEFAULT,MIN_MAGIC_NUMBER);
        } else printf(RED"\nError! Invalid input.\n"DEFAULT), clear_stdin();
    } else printf(RED"\nError! Values at minimum. Minimum allowed magic number: %d\n"DEFAULT,MIN_MAGIC_NUMBER);
}

/* **************************************************************************************************************************************************************************
 * Randomly Increase Magic Square - Increases all magic square numbers by a random amount (between 1 [MIN_RAND] and 10 [MAX_RAND]) to create a new magic square.            *
 * Verifies no values in the magic square will exceed the upper limit (MAX_MAGIC_NUMBER) the same way 'Increase Magic Square' function does.                                *
 * *************************************************************************************************************************************************************************/
void random_increase_ms(int **ms, int n) {
    int current_max = get_current_largest(ms,n), amount;
    if (current_max != MAX_MAGIC_NUMBER) {                  // If values not already at a maximum, seed random number generator, generate random, increase all values
        time_t t; srand((unsigned) time(&t));               // If the random num will cause any value to go above MAX_MAGIC_NUMBER, generate new random num
        while (((amount = MIN_RAND + rand() % MAX_RAND) + (current_max)) > MAX_MAGIC_NUMBER);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ms[i][j] += amount;                         // Once an acceptable value is generated, increase all values by that amount
            }
        }                                                   // Otherwise, values already at maximum, print error
    } else printf(RED"\nError! Values at maximum. Maximum allowed magic number: %d\n"DEFAULT,MAX_MAGIC_NUMBER);
}

/* **************************************************************************************************************************************************************************
 * Randomly Decrease Magic Square - Decreases all magic square numbers by a random amount (between 1 [MIN_RAND] and 10 [MAX_RAND]) to create a new magic square.            *
 * Verifies no values in the magic square will exceed go below 1 the same way 'Decrease Magic Square' function does.                                                        *
 * *************************************************************************************************************************************************************************/
void random_decrease_ms(int **ms, int n) {
    int default_center = get_original_center(n), current_center = get_current_center(ms,n), amount;
    if (current_center != default_center) {                 // If values not already at a minimum, seed random number generator, generate random, decrease all values
        time_t t; srand((unsigned) time(&t));               // If the random num will cause any value to go below 1, generate a new random number */
        while ((amount = MIN_RAND + rand() % MAX_RAND) > (current_center - default_center));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ms[i][j] -= amount;                         // Once an acceptable value is generated, increase all values by that amount
            }
        }                                                   // Otherwise, values already at minimum, print error
    } else printf(RED"\nError! Values at minimum. Minimum allowed magic number: %d\n"DEFAULT,MIN_MAGIC_NUMBER);
}

/* **************************************************************************************************************************************************************************
 * Max Increase Magic Square - Increases the magic square's numbers by the maximum amount (so that largest magic number = MAX_MAGIC_NUMBER) to create a new magic square.   *
 * *************************************************************************************************************************************************************************/
void max_increase_ms(int **ms, int n) {
    int amount = get_max_increase(ms,n);
    if (amount) {                                           // If amount != 0, increase each value by max amount
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ms[i][j] += amount;
            }
        }                                                   // Otherwise, amount==0 (values already at maximum), print error
    } else printf(RED"\nError! Values at maximum. Maximum allowed magic number: %d\n"DEFAULT,MAX_MAGIC_NUMBER);
}

/* **************************************************************************************************************************************************************************
 * Max Decrease Magic Square - Decreases the magic square's numbers by the maximum amount (so that lowest magic number = 1) to create a new magic square.                   *
 * *************************************************************************************************************************************************************************/
void max_decrease_ms(int **ms, int n) {
    int amount = get_max_decrease(ms,n);
    if (amount) {                                           // If amount != 0, decrease each value by maximum amount
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ms[i][j] -= amount;
            }
        }                                                   // Otherwise, amount==0 (values already at minimum/original), print error
    } else printf(RED"\nError! Values at minimum. Minimum allowed magic number: %d\n"DEFAULT,MIN_MAGIC_NUMBER);
}

/* **************************************************************************************************************************************************************************
 * Reset Magic Square - Resets the magic square to it's original state by refilling it                                                                                      *
 * *************************************************************************************************************************************************************************/
void reset_ms(int **ms, int n) {
    /* Reset all values to zero */
    for (int i = 0; i < n*n; i++) {
        ms[i/n][i%n] = 0;
    }
    /* Re-fill the magic square (same way it was created) */
    for (int i = 1, r = 0, c = n/2; i <= n*n; i++) {
        (!ms[r][c]) ? (ms[r][c] = i, r = (r+n-1)%n, c = (c+1)%n) : (ms[(r+2)%n][(c+n-1)%n] = i, r = (r+1)%n);
    }
}

/* **************************************************************************************************************************************************************************
 * Display Menu - Displays all menu options and prompts user for a selection. (Menu displayed in magenta)                                                                   *
 * *************************************************************************************************************************************************************************/
void display_menu(void) {
    printf("\n-------- Create a New Magic Square by Selecting an Option --------\n");
    printf("0.  Exit\n1.  Reverse Rows\n2.  Reverse Columns\n");
    printf("3.  Rotate 90d\n4.  Rotate 180d\n5.  Rotate 270d\n");
    printf("6.  Transpose\n7.  Reverse Transpose\n");
    printf("8.  Increase Values\n9.  Decrease Values\n");
    printf("10. Increase Values (Random Amount)\n11. Decrease Values (Random Amount)\n");
    printf("12. Increase Values (Max Amount)\n13. Decrease Values (Max Amount)\n");
    printf("14. Reset\n");
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
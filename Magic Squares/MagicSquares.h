/* =========================================================================================================================================================================*
 * Dave Dorzback                                                                                                                                                            *
 * MagicSquares.h                                                                                                                                                           *
 * =========================================================================================================================================================================*/

#ifndef MagicSquares_h
#define MagicSquares_h

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Macros: */

/* Macros for color escape sequences */
#if !COLOR_OFF
    #define DEFAULT "\033[0m"
    #define MAG "\x1b[35m"
    #define KCYN "\x1B[36m"
    #define RED "\x1b[31m"
#else
    #define DEFAULT ""
    #define MAG ""
    #define KCYN ""
    #define RED ""
#endif

/* Macro for swapping two distinct variables of the same type (XOR swap) */
#define swap(a, b) (a ^= b ^= a ^= b)

/* Macro for calloc */
#define alloc(n,ptr) (typeof(ptr)*)calloc(n,sizeof(ptr))

/* Macro for even integer test (even: true, odd: false) */
#define even(n) (n%2 ? 0 : 1)
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Prototypes: */

/* Prototype for Magic Square Size Input Function */
int *get_size(void);

/* Prototypes for Creating/Freeing Magic Square (using pointers) Functions */
int **create_ms(int n);
void free_ms(int **ms, int n);

/* Prototypes for Print Magic Square Function*/
void print_ms(int **ms, int n);

/* Prototype for Sum Magic Numbers Function */
int sum_ms(int **ms, int n);

/* Prototypes for Reversing Magic Square Functions */
void reverse_rows_ms(int **ms, int n);
void reverse_columns_ms(int **ms, int n);

/* Prototypes for Rotating Magic Square Functions */
void rotate_90_ms(int **ms, int n);
void rotate_180_ms(int **ms, int n);
void rotate_270_ms(int **ms, int n);

/* Prototypes for Transposing Magic Square Functions */
void transpose_ms(int **ms, int n);
void transpose_r_ms(int **ms, int n);

/* Prototypes for Increasing/Decreasing Magic Numbers Function */
void increase_ms(int **ms, int n);
void decrease_ms(int **ms, int n);
void random_increase_ms(int **ms, int n);
void random_decrease_ms(int **ms, int n);
void max_increase_ms(int **ms, int n);
void max_decrease_ms(int **ms, int n);

/* Prototype for Reset Magic Square Function */
void reset_ms(int **ms, int n);

/* Prototype for Display Menu Options Function */
void display_menu(void);
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif /* MagicSquares_h */

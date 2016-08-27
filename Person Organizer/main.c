/* ======================================================================================================================= *
 * Dave Dorzback                                                                                                           *
 * main.c                                                                                                                  *
 *                                                                                                                         *
 * This program accepts names and ages via command line arguments, copies them to an array of structs and prints the array *
 * sorted by name and then by age.                                                                                         *
 * Argument format is: name1 age1 name2 age2 ,..., nameN ageN.                                                             *
 * If missing names/ages are detected, the program will attempt to fill in the missing details.                            *
 * ======================================================================================================================= */
/* *********************************************** INCLUDES/DECLARATIONS ***************************************************/
/*** Includes ***/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*** Macros ***/
/* Macros for color escape sequences */
#if !COLOR_OFF                  // Set COLOR_OFF true to remove color from output
    #define DEFAULT "\033[0m"   // Default color
    #define MAG "\x1b[35m"      // Magenta
    #define RED "\x1b[31m"      // Red
    #define BLUE "\x1b[34m"     // Blue
    #define KCYN "\x1B[36m"     // Cyan
#else
    #define DEFAULT ""
    #define MAG ""
    #define RED ""
    #define BLUE ""
    #define KCYN ""
#endif

/* Macro for number of fields for person data (first name, age) */
#define NUM_FIELDS 2

/* Macros for radixsort -- sizes of bucket structures */
#define R 256   // R number of possible character values (0-255)
#define B 2     // B number of base 2 digit values (0-1)

/* Macros for sortPersonArray function */
#define SORT_BY_NAME 0
#define SORT_BY_AGE 1

/*** Typedefs ***/
/* 'Person' struct holds string for first name and int for age */
typedef struct {
    char *first_name;
    int age;
} Person;

/*** Prototypes ***/
/* printPersonArray - Prints info stored in array of Person structs */
void printPersonArray(char *message, Person *person_array, int n);

/* sortPersonArray - Sorts array of person structs by their names or by their ages */
void sortPersonArray(Person *person_array, int n, int sort_by);

/* getMaxPersonValue - Used by sortPersonArray - finds max name length/max age in Person array (depending on value of seach_by) */
int getMaxPersonValue(Person *person_array, int n, int search_by);

/* Prototypes for Person array memory allocation */
Person *createPersonArray(int n);                               // Allocates memory for an array of n Person's
Person *reallocPersonArray(Person *person_array, int new_size); // Reallocates memory for Person array to new_size Person's
void deletePersonArray(Person *person_array);                   // Frees memory used by Person array
/* *************************************************************************************************************************/

/* ******************************************************** MAIN ***********************************************************/
int main(int argc, const char *argv[]) {
    
    /* Define variables */
    Person *people;
    const char *no_name = "\0";
    int n = 0, offset = 0, allocd_mem = (argc-1)/NUM_FIELDS;
    
    /* Print error and exit if no arguments were entered */
    if (argc==1) printf(RED"Error! Not enough arguments.\nUsage: ./hw5 name1 age1 name2 age2 ...\nNow Exiting...\n"DEFAULT), exit(0);
    
    /* Create array of person structs. Starts with argc-1/NUM_FIELDS elements. If no arguments are missing, array will not be resized. */
    people = createPersonArray(allocd_mem);
    
    /* Analyze arguments and save info to Person array. Instead of exiting, program will attempt to detect missing arguments (names/ages) and fill them in with 0 */
    for (int i = 1; i < argc; i++) { // Iterates through each argument and keeps track of whether it should be a name or age (using offset and % NUM_FIELDS)
        if ((i+offset)%NUM_FIELDS) { /** Expect name argument **/
            if (isalpha(*argv[i])) {                    /* Argument is a name (assumes name if leading letter) */
                people[n].first_name = ((char*) argv[i]);       // Save the current argument as the current person's name
            } else {                                    /* Argument is NOT a name */
                people[n].first_name = ((char*) no_name);       // Set current person's name to null string
                if ((people[n].age=atoi(argv[i])) <= 0) {       // Save the current argument as current person's age
                    printf(RED"Warning: Invalid age (argument %d).\n"DEFAULT,i);    // If age invalid/negative, print warning message and set age to 0
                    people[n].age = 0;
                }                                                // Adjust offset & n to keep track of out of place args and current position
                ++offset;                                        // If extra space is needed, double the array size (reduced as needed later)
                if (++n==allocd_mem && i!=argc-1) people = reallocPersonArray(people,(allocd_mem*=2));
            }
        } else {                      /** Expect age argument **/
            if (isdigit(*argv[i])) {                    /* Argument is an age (assume age if leading digit */
                if ((people[n].age=atoi(argv[i])) <= 0) {       // Save the current argument as current person's age
                    printf(RED"Warning: Invalid age (argument %d).\n"DEFAULT,i);
                    people[n].age = 0;
                }
                if (++n==allocd_mem && i!=argc-1) people = reallocPersonArray(people,(allocd_mem*=2));
            } else {                                    /* Argument is NOT an age */
                people[n].age = 0;                              // Set current person's age to 0
                ++offset;                                       // Move to the next person and expand the array if needed
                if (++n==allocd_mem) people = reallocPersonArray(people,(allocd_mem*=2));
                people[n].first_name = ((char*) argv[i]);       // Save the current argument as next person's name
            }
        }
    }
    
    /* Adjust n (size of array) and set last person's age to 0 if last argument was a name (no age was entered for them) */
    if (isalpha(*argv[argc-1])) people[n++].age = 0;
    
    /* Print warning message if missing arguments were detected and adjust array size to avoid wasted memory */
    if (n != (argc-1)/NUM_FIELDS) {
        printf(RED"Warning: Missing argument(s). Input may be misinterpreted.\nUsage: ./hw5 name1 age1 name2 age2 ...\n"DEFAULT);
        people = reallocPersonArray(people,n);       // Array size decreased to n (number of persons)
    }
    
    /* Print the unsorted Person array */
    printf(BLUE"You have entered %d person(s) into the program.\n",n);
    printPersonArray("Unsorted Names:",people,n);
    
    /* Sort the Person array based on names, then print Person array */
    sortPersonArray(people,n,SORT_BY_NAME);
    printPersonArray(KCYN"Sorted by First Name:",people,n);
    
    /* Sort the Person array based on age, then print Person array */
    sortPersonArray(people,n,SORT_BY_AGE);
    printPersonArray(MAG"Sorted by Age:",people,n);
    
    /* Free array memory */
    deletePersonArray(people);
    
    /* Exit */
    printf(DEFAULT"Now Exiting...\n");
    return 0;
}
/* ****************************************************** END MAIN *********************************************************/

/* ***************************************************** FUNCTIONS *********************************************************/
/* **************************************************************************************************************************
 * printPersonArray - Accepts a string, pointer to Person array and integer for size of Person array.                       *
 * Prints the string and a newline first (if string is not null), then prints each name and age in Person array.            *
 * **************************************************************************************************************************/
void printPersonArray(char *message, Person *person_array, int n) {
    if (message) printf("%s\n",message);
    for (int i = 0; i < n; i++) {   // Prints "No Name" if empty string and "No Age" if no/invalid age
        printf("Name: %-10s |    Age: ",*person_array[i].first_name ? person_array[i].first_name : "No Name");
        person_array[i].age ? printf("%-10d\n",person_array[i].age) : printf("%-10s\n","No Age");
    }
}

/* **************************************************************************************************************************
 * sortPersonArray - Accepts an array of Person structs, an integer for its size, and an integer to specify which member to
 * sort the array by. 0 - sort structs by first_name member, 1 - sort structs by age member.
 * For sorting by name, LSD radixsort is used to sort the first_name strings. For sorting by age, binary MSD radixsort is used.
 * Neither implementation sorts the array in place, so extra memory is needed for temp array (MSD radixsort could be altered
 * to sort in place). However, both sorts are stable implementations. Names are sorted in case sensitive manner (for larger
 * sets of strings it might be faster to reduce number of buckets, R, by ignoring case/using a smaller alphabet).
 *
 * Refrences used:
 * - Radix sort. (2016, April 06). Retrieved from https://en.wikipedia.org/wiki/Radix_sort
 * - Counting sort. (2016, May 22). Retrieved from https://en.wikipedia.org/wiki/Counting_sort
 * - Eppstein, D. (1996, January 23). Bucket and radix sorting. Retrieved from https://www.ics.uci.edu/~eppstein/161/960123.html
 * - Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (1990). Introduction to algorithms (3rd ed.).
 * **************************************************************************************************************************/
void sortPersonArray(Person *person_array, int n, int sort_by) {
    
    if (sort_by!=0 && sort_by!=1) { // Print error and return if invalid sort option
        printf(RED"Error! Invalid sort option (0 - sort by name, 1 - sort by age).\n"MAG);
        return;
    }
    
    int d = getMaxPersonValue(person_array,n,sort_by);   // Max name length/age
    uint8_t key[n];                                      // Keys for bucket indexes
    Person tmp[n];                                       // Temp Person array
    
    if (!sort_by) { /* lsd radix sort */
        for (int idx = d-1; idx >= 0; idx--) {  // Start at last character in longest string, count sort chars at given index
            int b_[R] = {0};                    // Initialize buckets (256 buckets: 0-255 char values)
            for (int i = 0; i < n; i++) {       // Increment given bucket once for each character occurrence
                ++b_[key[i]=person_array[i].first_name[idx]];
            }
            for (int i = 1; i < R; i++) {       // Calculate the starting index for each bucket
                b_[i] += b_[i-1];
            }
            for (int i = n - 1; i >= 0; i--) {  // Save structs in sorted order to tmp array
                *(tmp + --b_[key[i]]) = person_array[i];
            }
            for (int i = 0; i < n; i++) {       // Save sorted values to the original array
                person_array[i] = tmp[i];
            }
        }
    } else {        /* msd (binary) radix sort */
        for (int bit_idx = 0; (d >> bit_idx) > 0; bit_idx++) {  // Start at first bit, count sort bits at given index
            int b_[B] = {0};                                    // Initialize bucket structure - only 2 - 0's and 1's buckets
            for (int i = 0; i < n; i++) {                       // Increment given bucket for each occurrence of digit
                ++b_[key[i]=((person_array[i].age >> bit_idx) & 1)];
            }
            b_[1] += b_[0];                                     // Calculate starting index for each bucket
            for (int i = n - 1; i >= 0; i--) {                  // Save the structs in sorted order to tmp
                *(tmp + --b_[key[i]]) = person_array[i];
            }
            for (int i = 0; i < n; i++) {                       // Copy sorted values back to original array
                person_array[i] = tmp[i];
            }
        }
    }
}

/* getMaxPersonValue - Accepts array of Person structs and returns the max name length/max age value in array. */
int getMaxPersonValue(Person *person_array, int n, int search_by) {
    int max = (!search_by) ? (int)strlen(person_array[0].first_name) : person_array[0].age, len;
    if (!search_by) {                       // search_by == 0, find the length of longest name
        for (int i = 1; i < n; i++) {       // Save and return value of length of longest name
            if (max < (len=(int)strlen(person_array[i].first_name))) max = len;
        }
        return max;
    } else {                                // search_by != 0, find value of largest age
        for (int i = 1; i < n; i++) {       // Save & return value of largest age
            if (max < person_array[i].age) max = person_array[i].age;
        }
        return max;
    }
}

/* createPersonArray - Accepts an integer for number of Person structs to allocate space for, allocates array of Persons and returns array address. */
Person *createPersonArray(int n) {
    Person *person_array = calloc(n,sizeof(*person_array));
    if (!person_array) {                // Print error and exit if calloc fails
        printf(RED"Error! Calloc failed, could not allocate memory."DEFAULT"\nNow Exiting...\n"), exit(0);
    } else return person_array;
}

/* reallocPersonArray - Accepts pointer to Person array, reallocates memory for the array to hold new_size Persons and returns new array address */
Person *reallocPersonArray(Person *person_array, int new_size) {
    Person *new_person_array = realloc(person_array,sizeof(*person_array)*new_size);
    if (!new_person_array) {            // Print error and exit if realloc fails
        printf(RED"Error! Realloc failed, could not allocate memory."DEFAULT"\nNow Exiting...\n"), exit(0);
    } else return new_person_array;
}

/* deletePersonArray - Accepts pointer to Person array and frees memory used by the array */
void deletePersonArray(Person *person_array) {
    free(person_array);
}
/* ******************************************************************************************************************************/
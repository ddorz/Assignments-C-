/* ======================================================================================================================= *
 * Dave Dorzback                                                                                                           *
 * main.c                                                                                                                  *
 *                                                                                                                         *
 * Input: Triangle coordinates (three sets of cartesian coordinates).                                                      *
 * Output: Rotated/reflected/translated triangle coordinates & triangle data (angles, area, sides, etc.).                  *
 * Note: Use of math.h functions not allowed.                                                                              *
 * ======================================================================================================================= */
/* *********************************************** INCLUDES/DECLARATIONS ***************************************************/
/*** Includes ***/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*** Macros ***/
/* Macros for color escape sequences. Errors printed in red, other text in cyan */
#if !COLOR_OFF                          // Set COLOR_OFF to remove color from output
    #define DEFAULT "\033[0m"           // Default color
    #define KCYN "\x1B[36m"             // Cyan
    #define RED "\x1b[31m"              // Red
    #define BLUE "\x1b[34m"
#else
    #define DEFAULT ""
    #define KCYN ""
    #define RED ""
    #define BLUE ""
#endif

/* Macro for theta (uppercase) symbol escape sequence */
#define THETA "\u03F4"

/* Macro for malloc -- n = number of blocks of sizeof(ptr) bytes to allocate. p = pointer to allocate memory for */
#define m_alloc(n,p) malloc(sizeof(p)*n)

/* Marco for triangle array sizes - triangle[N][M] | N - number of points in triangle | M - number of coordinates in point (x,y) */
#define N 3
#define M 2

/* Macro for size of user input string */
#define INPUT_LIMIT 26

/*** Function Prototypes ***/
int **createTriangle(void);

void freeTriangle(int **triangle);

void clockwiseRotateTriangle(int **triangle, int **rotated_triangle);

int reflectTriangle(int **triangle, int **reflected_triangle, char axis);

void translateTriangle(int **triangle, int **translated_triangle, int dx, int dy);

void printTriangle(int **triangle, char *message);

int isLine(int **n);

int checkStdin(void);

//=== Additional Triangle Function Prototypes ===//
void findAngles(int **triangle, float *triangle_angles);

void findSideLengths(int **triangle, float *side_lengths);

float findArea(int **triangle);

float findPerimeter(int **triangle);

float findSemiPerimeter(int **triangle);

void printTriangleData(float *triangle_data, char *typeof_data_str);

//=== Math/Vector Function Prototypes ===//
void getVector(int *p, int *q, int *u, int dim);

float norm(int *u, int dim);

int dotProduct(int *u, int *v, int dim);

void crossProduct(int *u, int *v, int *cross_p);

float fsqrt(float x);           // Replacement for math.h 'sqrtf'

float arccos(float x);          // Replacement for math.h 'acos'

float powF(float x, int e);     // Replacement for math. 'powf'

/******** Misc. Functions ********/
/* Function - converts uppercase character (A-Z) to its lowercase version (a-z). Returns non-letter characters unchanged */
char toLower(char c) { return (c >= 'A' && c <= 'Z') ? c | 0x60 : c; }               // Replacement for ctype.h 'tolower'

/* Function - determines the length of a given string. Returns string length */
int strLen(char *str) { int idx = 0; while (str[idx] && ++idx); return idx; }        // Replacement for string.h 'strlen'
/* *************************************************************************************************************************/

/* ******************************************************** MAIN ***********************************************************/
int main() {
    
    int **input_triangle, **transformed_triangle;   // pointers to int* for holding triangle coordinates
    char input_string[INPUT_LIMIT], *s1, *s2, axis; // strings for holding/converting user input & char for holding axis
    int dx, dy;                                     // integers for holding translation values
    float angles[N], side_lengths[N];               // arrays for hold angles/side lengths of starting triangle
    
    // Allocate memory for triangle coordinates & verify success. Set errno = 0 if successful, otherwise print error & exit
    if ((input_triangle=createTriangle()) && (transformed_triangle=createTriangle())) errno = 0;
    else perror(RED"Error"), printf(KCYN"\nNow Exiting...\n"DEFAULT), exit(1);
    
    // Input/Validate User Input- Get input as string (using fgets) and convert to numerical values for x & y (using strtol)
    for (int i = 0; i < N; i++) {
        printf(KCYN"Enter point #%d as x and y: ",i+1);
        fgets(input_string,INPUT_LIMIT,stdin);
        input_triangle[i][0] = ((int)strtol(input_string,&s1,10));  // Use 'input_string' for 1st strtol call and remaining string, 's1' for 2nd
        input_triangle[i][1] = ((int)strtol(s1,&s2,10));            // Cast results to int type since strtol returns long type
        if (!errno && input_string[strLen(input_string)-1]!='\n') errno = E2BIG;    // Verify input string wasn't too large
        else if (!errno && i==N-1 && isLine(input_triangle)) errno = EDOM;          // Verify input points do not form a line (checks only after last input)
        else if (!errno) {
            for (int j = 0; j < strLen(s2); j++) {                                  // Verify no extra (non-whitespace) characters input
                if (s2[j]!=' ' && s2[j]!='\t' && s2[j]!='\n') errno = EINVAL;
            }
        } // Print error message & exit if input is not valid (errno has non-zero value)
        if (errno) perror(RED"Error! Invalid Input"), printf(KCYN"\nNow Exiting...\n"DEFAULT), exit(1);
    }
    
    // Find/print triangle information (side length, angles, etc.)
    findSideLengths(input_triangle,side_lengths);   findAngles(input_triangle,angles);
    printTriangleData(side_lengths,"Side Length");  printTriangleData(angles,"Approx. "THETA);
    printf(BLUE"Area = %.2f, Perimeter = %.2f, Semiperimeter = %.2f\n"KCYN,findArea(input_triangle),findPerimeter(input_triangle),findSemiPerimeter(input_triangle));
    
    // Print the starting triangle (user input triangle) using printTriangle function
    printTriangle(input_triangle,"Starting triangle: ");
    
    // Rotate the triangle clockwise & print new triangle
    clockwiseRotateTriangle(input_triangle,transformed_triangle);
    printTriangle(transformed_triangle,"Rotated Triangle: ");
    
    // Reflect the triangle over given axis & print new triangle (if axis valid)
    printf("Enter axis to reflect (x or y): ");
    if (!(scanf(" %c",&axis)) || checkStdin())                              // Get/validate user input. Verify no extra non-whitespace characters entered
        axis = 0;                                                           // If input invalid, set axis to 0 (invalid axis value)
    if (!reflectTriangle(input_triangle,transformed_triangle,axis)) {
        printTriangle(transformed_triangle,"Reflected Triangle: ");         // If reflectTriangle returns 0 (axis was valid) => printTriangle
    } else {                                                                // Otherwise, if returns -1 => print error
        printf("Reflected Triangle: Invalid Axis. Next time enter x or y.\n");
    }
    
    // Translate the triangle by given amounts & print new triangle
    printf("Enter translation values (x and y): ");
    if (scanf("%d %d",&dx, &dy)!=2 || checkStdin()) {                        // Get/validate user input
        printf(RED"Warning! Invalid input. Values defaulting to x = 0 and y = 0.\n"KCYN);
        dx = dy = 0;                                                         // If input invalid, print warning and set values to 0
    }
    translateTriangle(input_triangle,transformed_triangle,dx,dy);            // Translate & print
    printTriangle(transformed_triangle,"Translated Triangle: ");
    
    // Free memory, print exit message & restore console color
    freeTriangle(input_triangle); freeTriangle(transformed_triangle);
    printf("\nNow Exiting...\n"DEFAULT);
    return 0;
}
/* ****************************************************** END MAIN *********************************************************/

/* ***************************************************** FUNCTIONS *********************************************************/
/* **************************************************************************************************************************
 * createTriangle - Accepts pointer to pointer to int and returns the same type after allocating memory for it.             *
 * Allocates memory using malloc so pointer can be indexed as nxm 2d array.                                                 *
 * **************************************************************************************************************************/
int **createTriangle(void) {
    int **triangle = m_alloc(N,*triangle), i;   // First create and allocate array of n pointers (N blocks of int* size)
    if (!triangle) {                            // If malloc fails (returns no address), return NULL
        return NULL;
    }
    for (i = 0; i < N; i++) {
        triangle[i] = m_alloc(M,*triangle[i]);  // Then allocate space for each of the row's buckets (M blocks of int size)
        if (!triangle[i]) {                     // If any malloc fails, free any allocated memory and return NULL
            freeTriangle(triangle);
            return NULL;
        }
    }
    return triangle;                            // Return the address of the allocated nxm 'triangle' array
}

/* **************************************************************************************************************************
 * freeTriangle - Accepts pointer to pointer to int (array for holding triangle points). No return value.                   *
 * Frees the memory allocated for the given triangle array in the opposite order it was allocated (rows first).             *
 * **************************************************************************************************************************/
void freeTriangle(int **triangle) {
    for (int i = 0; i < N; i++) {
        free(triangle[i]);          // Free memory for each of the 3 rows
    }
    free(triangle);                 // Free the memory for the array of rows
}

/* **************************************************************************************************************************
 * printTriangle - Accepts int** (array holding triangle's coordinates) and char pointer (string). No return value.         *
 * Prints the given string followed by the coordinates in the given triangle array (in the form (x1, y1)(x2, y2)...)        *
 * When printing values stored in array parameter, iterates through the array using pointer arithmetic.                     *
 * **************************************************************************************************************************/
void printTriangle(int **triangle, char *message) {
    printf("%s",message);
    for (int i = 0, *p; i < N; i++) {   // The allocated memory may not be contiguous, but each row will be, so start at the-
        p = triangle[i];                // -base of each row and iterate through it's elements using pointer arithmetic.
        for (int j = 0; j < M; j++) {   // Outer loop runs N iterations (for each row), inner loop runs M iterations (for each element)
            printf(!j ? "(%d, " : "%d)",*p); // Print commas/parenthesis w/ value according to position in row (x or y => 0 or 1)
            p++;
        }
    }
    printf("\n");
}

/* **************************************************************************************************************************
 * clockwiseRotateTriangle - 1st parameter: int** (array holding triangle's coordinates).                                   *
 * 2nd parameter: int** (array to hold rotated triangle's coordinates). No return value.                                    *
 * Takes the coordinates of a triangle and generates new coordinates for the clockwise rotated triangle.                    *
 * **************************************************************************************************************************/
void clockwiseRotateTriangle(int **triangle, int **rotated_triangle) {
    for (int i = 0; i < N; i++) {                   // Loop runs 3 iterations, once for each coordinate
        rotated_triangle[i][0] = triangle[i][1];    // Get x value for rotated triangle (original triangle's y value)
        rotated_triangle[i][1] = -triangle[i][0];   // Get y value for rotated triangle (original triangle's x value * -1)
    }
}

/* **************************************************************************************************************************
 * reflectTriangle - 1st parameter: int** (array holding triangle's coordinates).                                           *
 * 2nd parameter: int** (array to hold reflected triangle's coordinates).                                                   *
 * 3rd parameter: char for holding axis value (x or y).                                                                     *
 * Returns 0 if successful (axis parameter valid) or -1 if unsuccessful (axis parameter invalid).                           *
 * Takes the coordinates of a triangle and generates new coordinates for the reflected triangle.                            *
 * **************************************************************************************************************************/
int reflectTriangle(int **triangle, int **reflected_triangle, char axis) {
    if ((axis=toLower(axis))!='x' && axis!='y') return -1;                  // Return -1 if axis is not valid (x, X, y, Y)
    for (int i = 0; i < N; i++) {                                           // Loop runs 3 iterations, once for each coordinate
        reflected_triangle[i][abs('x'-axis)] = triangle[i][abs('x'-axis)];  // x-x= 0, x-y= -1 | If x-axis: x2 = x1, y2 = -y1
        reflected_triangle[i]['y'-axis] = -triangle[i]['y'-axis];           // y-y= 0, y-x= 1  | If y-axis: x2 = -x1, y2 = y1
    }
    return 0;
}

/* **************************************************************************************************************************
 * translateTriangle - 1st parameter: int** (array holding triangle's coordinates).                                         *
 * 2nd parameter: int** (array to hold translated triangle's coordinates).                                                  *
 * 3rd/4th parameters: ints for holding translation amounts (x amount & y amount). No return value.                         *
 * Takes the coordinates of a triangle and generates new coordinates for the translated triangle.                           *
 * **************************************************************************************************************************/
void translateTriangle(int **triangle, int **translated_triangle, int dx, int dy) {
    for (int i = 0; i < N; i++) {                        // Loop runs 3 iterations, once for each coordinate
        translated_triangle[i][0] = triangle[i][0] + dx; // Get x for translated triangle by adding input amount to original x
        translated_triangle[i][1] = triangle[i][1] + dy; // Get y for translated triangle by adding input amount to original y
    }
}

/* **************************************************************************************************************************
 * isLine - For input validation - 1st parameter: int** (triangle's coordinates).                                           *
 * Returns - true (positive value) if coordinates form a line, or false (0 value) if not.                                   *
 * **************************************************************************************************************************/
int isLine(int **n) {
    int v = 1, h = 1, d = 1;
    for (int i = 0; i < N-1; i++) {                                    // Return 1 if any duplicate coordinates
        if ((n[i][0]==n[i+1][0] && n[i][1]==n[i+1][1]) || (n[i][0]==n[(i+2)%N][0] && n[i][1]==n[(i+2)%N][1])) return 1;
        if (n[i][0]!=n[i+1][0]) h = 0;                                 // h = 0 indicates points do not form horizontal line
        if (n[i][1]!=n[i+1][1]) v = 0;                                 // v = 0 indicates points do not form vertical line
        if (((float)n[i+1][1]-n[i][1])/n[i+1][0]-n[i][0]!=1.0f) d = 0; // d = 0 indicates points do not form diagonal line
    }
    return v+h+d;
}

/* **************************************************************************************************************************
 * checkStdin - For input validation - Reads input in stdin (30 chars at a time) and checks the characters.                 *
 * Ignores whitespace characters (' ', '\t', '\n'), but returns true (1) if any others characters found in buffer.          *
 * If stdin is empty or only contains whitespace characters, returns false (0).                                             *
 * **************************************************************************************************************************/
int checkStdin(void) {
    char buff[30], c = 0;
    while (buff[strLen(buff)-1]!='\n' && fgets(buff,30,stdin)!=NULL) {
        for (char *p = buff; *p; ++p) {
            c = (!c && *p!=' ' && *p!='\t' && *p!='\n') ? 1 : c;
        }
    }
    return c;
}

//============================================== Additional Triangle Functions =============================================//

/* findAngles - Takes the coordinates for a triangle and finds its (approximate) angles */
void findAngles(int **triangle, float *triangle_angles) {
    int side_vecs[M][M];
    const float DEG = 180.0f / 3.14159f;    // Conversion factor for radians to degrees
    for (int i = 0; i < N; i++) {
        getVector(triangle[i],triangle[(i+1)%N],side_vecs[0],M);                // Find side vectors
        getVector(triangle[i],triangle[(i+2)%N],side_vecs[1],M);                // Then find the angle between the two vectors
        triangle_angles[i] = arccos((dotProduct(side_vecs[0],side_vecs[1],M))/(norm(side_vecs[0],M)*norm(side_vecs[1],M)));
        triangle_angles[i] =((int)(triangle_angles[i]*DEG * 10+0.5f) / 10.0f);  // Convert to degrees and round to 1 decimal
        
    }
}

/* findSideLengths - Takes the coordinates for a triangle and finds its (approximate) side lengths */
void findSideLengths(int **triangle, float *side_lengths) {
    int side[M];
    for (int i = 0; i < N; i++) {
        getVector(triangle[i],triangle[(i+1)%N],side,M); // Find vectors between the vertices
        side_lengths[i] = norm(side,M);                  // Find the magnitude of each vector
    }
}

/* findArea - Takes the coordinates for a triangle and returns the approximate area of the triangle */
float findArea(int **triangle) {
    int ab[3], ac[3], c_prod[3];
    getVector(triangle[0],triangle[1],ab,M); // Find vector AB
    getVector(triangle[0],triangle[2],ac,M); // Find vector AC
    ab[2] = ac[2] = 0;
    crossProduct(ab,ac,c_prod);              // Find AB x AC (|AB x AC| = 2x area of the triangle)
    return norm(c_prod,3) / 2;               // Return 1/2 the magnitude of AB x AC
}

/* findPerimeter - Takes the coordinates for a triangle and returns the perimeter of the triangle */
float findPerimeter(int **triangle) {
    float side_lens[N], p = 0;
    findSideLengths(triangle, side_lens);    // Find the length of each side
    for (int i = 0; i < N; i++) {            // Sum the lengths
        p+=side_lens[i];
    }
    return p;
}

/* findSemiPerimeter - Takes the coordinates for a triangle and returns the semiperimeter of the triangle */
float findSemiPerimeter(int **triangle) {
    return findPerimeter(triangle) / 2;      // Find half of the perimeter (sp = a+b+c / 2 = p / 2)
}

/* printTriangleData - Takes an array holding triangle and a string for the name of the data and prints the information */
void printTriangleData(float *triangle_data, char *typeof_data_str) {
    for (int i = 0; i < N; i++) {
        printf(BLUE"%s #%d = %.2f%s",typeof_data_str,i+1,triangle_data[i],((i+1)%N) ? ", " : ("\n"KCYN));
    }
}

//============================================== Math/Vector Functions ====================================================//

/* getVector - Takes an initial point (p), terminal point (q) and calculates the vector (u) between them */
void getVector(int *p, int *q, int *u, int dim) {
    for (int i = 0; i < dim; i++) {
        u[i] = q[i] - p[i];                  // Calculate each vector component
    }
}

/* norm - Takes an array holding vector components and its dimension and calculates the vector's magnitude */
float norm(int *u, int dim) {
    float x = 0;
    for (int i = 0; i < dim; i++) {
        x += (u[i] * u[i]);                  // Square each component and sum them
    }
    return fsqrt(x);                         // Return the square root of the sum of squares of components
}

/* dotProduct - Takes two vectors and their dimension and calculates their dot product using their components */
int dotProduct(int *u, int *v, int dim) {
    int dot_p = 0;
    for (int i = 0; i < dim; i++) {          // One iteration for each component
        dot_p += (u[i] * v[i]);              // Multiply components and sum them
    }
    return dot_p;
}

/* crossProduct - Takes two vectors and calculates their cross product using their components (all parameters must have length 3) */
void crossProduct(int *u, int *v, int *cross_p) {
    for (int i = 0, idx = 2; i < 3; i++) {                                  // Cross product only defined for R^3
        for (int j = i+1; j < 3; j++, idx--) {                              // Calculates cross product vector (in reverse order) using column-row expansion
            cross_p[idx] = (u[i] * v[j] - v[i] * u[j]) * powF(-1,idx);
        }
    }
}

/* fsqrt - approximates the square root of a given value, x */
float fsqrt(float x) {
    float a = x, b = 1.0f;                                                  // Upper/lower bounds for range of possible sqrt values
    float lim = (x>9600000.f) ? 0.1f : (x>95000.f) ? 0.0001f : 0.000001f;   // Error bound (larger values require a larger margin)
    do {
        b = (b + a) / 2.0f;                                                 // Set upper bound to middle of current range
        a = x / b;                                                          // Set lower bound to x / upper bound
    } while ((b-a) > lim || (b-a) < -lim);                                  // Continue until the bounds are approximately equal to sqrt(x)
    return b;
}

/* arccos - approximates the inverse cosine for a given value, x */
float arccos(float x) {
    if (x>1.0f || x<-1.0f) return -1;       // Range: -1.0 < x < 1.0. More accurate approximation used for x values -0.2 < x < 0.2, but uses more calculations
    else if (x>0.2f || x<-0.2f) return (8.0f/3.0f)*fsqrt(2.0f-fsqrt(2.0f+2.0f*x))-(1.0f/3.0f)*fsqrt(2.0f-2.0f*x);
    else return (256.0f/45.0f)*fsqrt(2.0f-fsqrt(2.0f+fsqrt(2.0f+2.0f*x))) - (8.0f/9.0f)*fsqrt(2.0f-fsqrt(2.0f+2.0f*x)) + (1.0f/45.0f)*fsqrt(2.0f-2.0f*x);
}

/* powF - raises a given float (x) by a given power (e) and returns the result */
float powF(float x, int e) {
    float s = 1;
    for (int i = 0; i < e; i++) {
        s*=x;
    }
    return s;
}
/* *************************************************************************************************************************/
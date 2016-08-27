/* ======================================================================================================================= *
 * Dave Dorzback                                                                                                           *
 * main.c                                                                                                                  *
 *                                                                                                                         *
 * Input: Quadratic expression coefficients (a, b, c).                                                                     *
 * Output: Quadratic function, real roots / complex roots (whichever applicable), vertex, directrix, direction of graph.   *
 * ======================================================================================================================= */
/* *********************************************** INCLUDES/DECLARATIONS ***************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Macros for color escape sequences. Errors printed in red, other text in cyan */
#if !COLOR_OFF                          // Set COLOR_OFF to remove color from output
    #define DEFAULT "\033[0m"           // Default color
    #define KCYN "\x1B[36m"             // Cyan
    #define RED "\x1b[31m"              // Red
#else
    #define DEFAULT ""
    #define KCYN ""
    #define RED ""
#endif

/* Macro for determining sign of given value, x. Expression result will be either '+ or '-' char (used with printf) */
#define sign_of(x) ((x < 0) ? '-' : '+')

/* Declare 'quadratic' struct for holding attributes of quadratics. Typedef so type can be declared with 'Quadratic' keyword */
typedef struct quadratic {
    int a, b, c;                        // Coefficients => f(x) = ax^2 + bx + c
    int num_roots;                      // Number of roots ('2' - 2 real roots, '1' - 1 real root, '0' - 2 complex roots)
    double r1, r2;                      // Roots values
    double x, y;                        // Vertex coordinates (x, y) => (h, k)
    double directrix;                   // Directrix
    int orientation;                    // Orientation of quadratic ('U' - opens up, 'D' - opens down)
} Quadratic;

/* Function prototypes */
int quadraticFormula(Quadratic *q1);    // Returns number of roots, saves values to q1's .r1 & .r2 members
void quadraticVertex(Quadratic *q1);    // No return, saves values to q1's .x & .y members
void quadraticInfo(Quadratic *q1);      // No return, saves values to q1's .orientation & .directrix members
/* *************************************************************************************************************************/

/* ******************************************************** MAIN ***********************************************************/
int main() {
    
    /* Define variables */
    int tmp, input_val;                 // Input validation variables
    Quadratic q1;                       // 'Quadratic' struct, holds values of the quadratic's attributes
    
    /* Prompt user and get input */
    printf(KCYN"Enter coefficients a, b and c: ");
    input_val = scanf("%d %d %d",&q1.a,&q1.b,&q1.c);
    
    /* Input validation: a!=0, scanf successful (returns 3), no extra characters (other than whitespace) entered */
    while ((tmp=getchar())!='\n' && input_val==3) {     // If extra characters are entered, but only whitespace, accept input as valid
        if (tmp!=' ' && tmp!='\t') input_val = 0;       // If extra non-whitespace characters entered, input invalid: set input_val = 0
    } /* Print error message & exit if input is not valid ('input_val' doesn't equal 3 or 'a' equals 0) */
    if (input_val!=3 || q1.a==0) {
        printf(RED"Error! Invalid Input (integers only, with a!=0, in the form: a b c).\n"KCYN"\nNow Exiting...\n"DEFAULT);
        exit(1);
    }
    
    /* Print quadratic - prints non-zero values only; with appropriate sign & a space before them */
    printf("f(x) = %dx^2 ",q1.a);
    if (q1.b) { printf("%c %dx ",sign_of(q1.b),abs(q1.b)); }
    if (q1.c) { printf("%c %d",sign_of(q1.c),abs(q1.c));   }     // e.g. prints: '- 4' instead of: '+ -4'
    printf("\n");
    
    /* Find number of real roots and their values by passing address of q1 to 'quadraticFormula', save return value */
    q1.num_roots = quadraticFormula(&q1);
    /* Print the value(s) of the roots accordingly (0: 2 complex roots | 1: 1 real root | 2: 2 real roots) */
    switch (q1.num_roots) {
        case 0:
            printf("No real roots\nComplex roots: ");
            printf("%.2f %c %.2fi and ",q1.r1,sign_of(q1.r2),fabs(q1.r2));     // complex root 1 = q1.r1 + (q1.r2)i
            printf("%.2f %c %.2fi\n",q1.r1,sign_of(-q1.r2),fabs(-q1.r2));      // complex root 2 = q1.r1 - (q1.r2)i
            break;
        case 1:
            printf("Root: %.2f\n",q1.r1);                                      // real root = q1.r1 = q2.r2
            break;
        case 2:
            printf("Roots: %.2f and %.2f\n",q1.r1,q1.r2);                      // real root 1 = q1.r1
            break;                                                             // real root 2 = q1.r2
    }
    
    /* Find the vertex of quadratic's graph by passing address of q1 to 'quadraticVertex' */
    quadraticVertex(&q1);
    /* Print the vertex values */
    printf("Vertex: (%.2f, %.2f)\n",q1.x,q1.y);
    
    /* Find the orientation and directrix of the quadratic's graph by passing address of q1 to 'quadraticInfo' */
    quadraticInfo(&q1);
    /* Print the orientation (up for U or down for D) and directrix values accordingly */
    printf("Directrix: y = %.2f\n",q1.directrix);
    printf("The graph is facing %s\n",(q1.orientation=='U') ? "up" : "down");
    
    /* Print exit message & restore console color */
    printf("\nNow Exiting...\n"DEFAULT);
    return 0;
}
/* ****************************************************** END MAIN *********************************************************/

/* ***************************************************** FUNCTIONS *********************************************************/
/* **************************************************************************************************************************
 * quadraticFormula - Accepts pointer to 'Quadratic' struct and returns integer for number of real roots (0, 1 or 2).       *
 * Determines the number of roots based on discriminant then finds the root values. If 0 real roots, 2 complex roots.       *
 * Saves the values of roots in the parameter's r1 & r2 member addresses.                                                   *
 * Note: If quadratic has 2 complex roots, they are complex conjugates, so real part will be the same for both. This value- *
 * is stored in q1.r1. The value for the 1st root's imaginary part stored is in q1.r2 and the 2nd root's imaginary value-   *
 * is given by -q1.r2.                                                                                                      *
 * **************************************************************************************************************************/
int quadraticFormula(Quadratic *q1) {
    /* Find discriminant of quadratic using formula D = b^2 / 4ac to determine the number of real roots */
    double discrim = pow(q1->b,2) - (4.0 * q1->a * q1->c);
    /* Determine number of real roots, then the values of the roots */
    if (discrim > 0) {          /* discriminant > 0: 2 real roots */
        q1->r1 = (-q1->b + sqrt(discrim)) / (2.0 * q1->a); // real root value 1 = -b + sqrt(D) / 2a
        q1->r2 = (-q1->b - sqrt(discrim)) / (2.0 * q1->a); // real root value 2 = -b - sqrt(D) / 2a
        return 2;
    } else if (discrim == 0) {  /* discriminant == 0: 1 real root */
        q1->r1 = q1->r2 = -q1->b / (2.0 * q1->a);          // real root value = -b / 2a
        return 1;
    } else {                    /* discriminant < 0: 0 real roots, 2 complex roots */
        q1->r1 = -q1->b / (2.0 * q1->a);                   // complex root values, real parts = -b / 2a
        q1->r2 = sqrt(fabs(discrim)) / (2.0 * q1->a);      // complex root values, imaginary parts = -sqrt(D)/2a & sqrt(D)/2a
        if (discrim < 0) q1->r2 *= -1.0;                   // fabs() to avoid negative under radical, sign adjusted as needed
        return 0;
    }
}

/* **************************************************************************************************************************
 * quadraticVertex - Accepts pointer to 'Quadratic' struct. Void return type.                                               *
 * Finds vertex coordinates of quadratic graph.                                                                             *
 * Saves the values for vertex coordinates (x and y) in the parameter's x & y member addresses.                             *
 * **************************************************************************************************************************/
void quadraticVertex(Quadratic *q1) {
    q1->x = -(q1->b / (2.0 * q1->a));                      // find x coordinate: x = -b / 2a
    q1->y = q1->c - (q1->a * pow(q1->x,2));                // find y coordinate: y = c - ax^2
}

/* **************************************************************************************************************************
 * quadraticInfo - Accepts pointer to 'Quadratic' struct. Void return type.                                                 *
 * Finds orientation ('U' - opens up, 'D' - opens down) and directrix of quadratic graph.                                   *
 * Saves the values for orientation and directrix in the parameter's orientation & directrix member addresses.              *
 * **************************************************************************************************************************/
void quadraticInfo(Quadratic *q1) {
    q1->orientation = (q1->a > 0) ? 'U' : 'D';             // a > 0: opens up, b < 0: opens down
    q1->directrix = -(pow(q1->b,2) - (4.0 * q1->a * q1->c) + 1.0) / (4.0 * q1->a); // directrix = -(b^2 - 4ac + 1) / 4a
}
/* *************************************************************************************************************************/
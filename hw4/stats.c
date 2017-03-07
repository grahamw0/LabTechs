/* Reads in an arbitrary amount of numbers, then displays statistics about
   them.
   Author: Will Graham <grahamw0@students.rowan.edu>
   Date: 9 September 2016
 */


#include <stdio.h>
#include <float.h>

int main()
{
    double item;
    char itemsread;
    double max = -DBL_MAX; // Allows for negatives to be largest number
    double min = DBL_MAX; // Allows for positives to be smallest number
    double sum = 0;
    int itemNumber = 0;
    double mean = 0;
    char c;

    while ( (itemsread = scanf("%lf", &item)) != EOF && item != 0) {
        if (itemsread == 1) {
            itemNumber++;
            sum += item;

            if (item > max) {
                max = item;
            }

            if (item < min) {  // 1st number can be both min and max
                min = item;
            }
        }

        while ( (c = getchar()) != '\n' )
            ; // do nothing
    }

    if (itemNumber != 0) {  // Protects against dividing by zero
        mean = sum / itemNumber;
    } else { // Overwrites initial values in case of no non-zero inputs
        max = 0;
        min = 0;
    }

    printf("# of items: %i\n", itemNumber);
    printf("Maximum: %f\n", max);
    printf("Minimum: %f\n", min);
    printf("Mean: %f\n", mean);
    printf("Sum: %f\n", sum);

    return 0;
}

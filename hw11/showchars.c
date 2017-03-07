/* showchars.c - Show characters
 *
 * D Provine, 20 March 2012
 *
 * Edited: Will Graham, Nov 2016
 *
 * COMPLETE: Shows more than one character!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chars/All_Chars.h"

int main(int argc, char *argv[])
{
    int line;
    int bit;
    int charnum;
    int arraypos;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s string\n", argv[0]);
        exit(1);
    }

    for (line=0; line < 10; line++) {
        int i;
        // each line has to be at least glyph size * # of glyphs + 1 for null char
        char *lineString = malloc(8 * strlen(argv[1]) + 1);
        for (i = 0; i < strlen(argv[1]); i++) {
            /* argv[1][0] is the first character of the first argument.
             * (The argument is a null-terminated string, even if the
             *  user only types one character.)
             * Converting it to an int gives us the ASCII value.
             */
            charnum = (int) argv[1][i];

            /* Our array starts at index 0, which is for the " " char.
             * The " " char has ASCII value 0x20, but array index zero,
             * so we have to subtract to get the array position.
             */
            arraypos = charnum - 0x20;

            for (bit = 7; bit >= 0; bit--) {
                if ( (*glyph[arraypos])[line] & ( 1 << bit ) )
                    strcat(lineString, "X");
                else
                    strcat(lineString, " ");
            }

        }
        printf("%s\n", lineString);
    }

    return 0;
}


/* stringscroll.c - scroll a string
 *
 * D Provine and Posse, 9 April 2015
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define DISPLAYSIZE   9
#define BUFSIZE       128
#define MAXMESSAGELEN (BUFSIZE - DISPLAYSIZE * 2 - 1)

int main(int argc, char *argv[])
{
    char message[BUFSIZE] = "";
    char visible[DISPLAYSIZE + 1]; // +1 for null
    int  i;
    int  step;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s string\n", argv[0]);
        exit(1);
    }

    if (strlen(argv[1]) > MAXMESSAGELEN ) {
        fprintf(stderr, "%s: message too long\n", argv[0]);
        exit(1);
    }

    sprintf(message,
            "%*s%.*s%*s%",
            DISPLAYSIZE, "",         // pad left of string
            MAXMESSAGELEN, argv[1],  // truncate message if needed
            DISPLAYSIZE, "");        // pad right of string

    step = 0;
    while ( 1 ) {
        sprintf(visible, "%.*s", DISPLAYSIZE, &(message[step]) );
        printf(" |%s|\r", visible);
        fflush(stdout);
        usleep(100000);
        if ( step < strlen(message) - DISPLAYSIZE ) {
            step++;
        } else {
            step = 0;
        }
    }

    return 0;
}


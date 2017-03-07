/* multiprez.c - Write Information About Multiple Presidents
 *
 * D Provine
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Obama is #44
#define MAXENTRIES 44

typedef struct {
    char           inits[5];
    unsigned char  num;
} prezinfo;

int main(int argc, char *argv[])
{
    prezinfo  person[MAXENTRIES];
    FILE     *saveit;
    int       numscanned;
    int       num;
    char      inits[5];
    int       thispres;
    int       i;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s FILENAME\n", argv[0]);
        exit(1);
    }

    // NOTE: NO ERROR CHECKING ON INITIALS!
    thispres = 0;
    while ( ( numscanned = scanf("%d", &num) ) != EOF && num != 0 ) {
        if (numscanned == 1) {
            // get the initals
            numscanned = scanf("%s", &inits[0]);

            person[thispres].num = num;
            strcpy(person[thispres].inits, inits);

            thispres++;
        }
        if (thispres > MAXENTRIES)
            break;
    }

    if ( ( saveit = fopen(argv[1], "w") ) == NULL ) {
        perror("pres.dat");
        exit(1);
    }


    for (i = 0; i < thispres; i++) {
        printf("%d\n", fwrite(&(person[i]), sizeof(prezinfo), 1, saveit));
    }

    // NOTE: an alternate way to do this could have been
    // printf("%d\n", fwrite(person, sizeof(prezinfo), thispres-1, saveit));
    // which would have written all of them at once.
    // The printf() would have printed however many were there.
    // This one will print "1" for each president.

    fclose(saveit);

    return 0;
}


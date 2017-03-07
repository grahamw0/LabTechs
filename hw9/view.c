/* view.c -- view module for clock project
 *
 * Darren Provine, 17 July 2009
 */

#include "clock.h"

/* VIEW */

/* We get a pointer to a "struct tm" object, put it in a string, and
 * then send it to the screen.
 */
void ledshow(struct tm *dateinfo, int ampm)
{
    char       timestring[9];
    digit *where = get_display_location();
    int i;

    /* Note that we have removed the colons to make this
     * part easier to write.
     */

    if(display_mode == MODE_DATE) {
        strftime(timestring, 9, "%m%d%y", dateinfo);
        where[6] = 0x00;
    } else if (display_mode == MODE_TEST) {
        strcpy(timestring, "888888");
        where[6] = 0xF0;
    } else {
        if(ampm) {
            // Format string as hh:MM:ss AM/PM, w/ no hour 0 padding
            strftime(timestring, 9, "%l%M%S", dateinfo);
            if(dateinfo->tm_hour >= 12) { // AM/PM marker
                where[6] = 0xf4; // PM
            } else {
                where[6] = 0xf2; // AM
            }
        } else {
            sprintf(timestring,
                    "%02d%02d%02d",
                    dateinfo->tm_hour,
                    dateinfo->tm_min,
                    dateinfo->tm_sec);
            where[6] = 0xf1;
        }
    }


    for (i = 0; i < 6; i++) {
        switch ( timestring[i] ) {
        case ' ':
            where[i] = 0x00;
            break;
        case '1':
            where[i] = 0x24;
            break;
        case '2':
            where[i] = 0x5d;
            break;
        case '3':
            where[i] = 0x6d;
            break;
        case '4':
            where[i] = 0x2e;
            break;
        case '5':
            where[i] = 0x6b;
            break;
        case '6':
            where[i] = 0x7b;
            break;
        case '7':
            where[i] = 0x27;
            break;
        case '8':
            where[i] = 0x7f;
            break;
        case '9':
            where[i] = 0x2f;
            break;
        case '0':
            where[i] = 0x77;
            break;
        }
    }

    display();
    fflush(stdout);
}

void show(struct tm *dateinfo, int ampm)
{
    char       timestring[12];

    // Format string as hh:mm:ss AM/PM, w/ no 0 padding
    if(ampm) {
        strftime(timestring, 12, "%l:%M:%S %p", dateinfo);
    } else {
        sprintf(timestring,
                "%02d:%02d:%02d",
                dateinfo->tm_hour,
                dateinfo->tm_min,
                dateinfo->tm_sec);
    }
    printf("\r%s ", timestring);
    fflush(stdout);

}

/* clock.c -- controller part of the clock project
 *
 * Darren Provine, 17 July 2009
 *
 */

#include "clock.h"

/* CONTROLLER */

static char bugaddress[]="grahamw0@students.rowan.edu";

// version -- say which version this is and exit
// (note simple output; you could also print the rcsid too)
void version()
{
    fprintf(stderr, "simple clock version 1\n");
    exit(0);
}

// usage -- print brief summary and instructions
void usage(char *progname)
{
    fprintf(stderr, "This program displays a realtime clock.\n");
    fprintf(stderr, "Usage: %s [-avh] [-o number]\n", progname);
    fprintf(stderr, "  -a    : am/pm instead of 24 hour\n");
    fprintf(stderr, "  -l    : use simulated LED display\n");
    fprintf(stderr, "  -o #  : offset the time by # seconds \n");
    fprintf(stderr, "  -v    : show version information\n");
    fprintf(stderr, "  -h    : this help message\n");
    fprintf(stderr, "report bugs to %s \n", bugaddress);
    exit (0);
}

/* These have to be global because tick() is called
 * automatically; we can't pass it any arguments because
 * we don't call it.
 */
int offset = 0;
int ampm = 0;
int LED  = 0;
int display_mode = MODE_CLOCK;
time_t mode_expires;  // Instr. say int, but that throws a lot of compilier warnings- this doesn't.

void process_key(keybits KeyCode)
{
    void stop_clock(void);
    int KeyRow, KeyCol;

    if (KeyCode & 0x80) {  // mouse hit

        // TODO: add code to figure out which row / column
        //       the button is in

        KeyRow = KeyCode & 0x10;
        KeyCol = KeyCode & 0x0F;

        if (KeyRow == 0) {
            switch (KeyCol) {
            case 0:
                ampm = 0;
                break;
            case 1:
                ampm = 1;
                break;
            case 2:
                display_mode = MODE_DATE;
                (void) time(&mode_expires);
                mode_expires += 5;
                break;
            case 3:
                display_mode = MODE_TEST;
                (void) time(&mode_expires);
                mode_expires += 5;
                break;
            case 4:
                stop_clock();
                break;
            }
        } else {
            char title_text[81];
            switch (KeyCol) {  // I'm not proud of this, but I find it funny.
            case 0:
                ;
                strcpy(title_text, "IN A CLOCK");
                set_title_bar(title_text);
                break;
            case 1:
                ;
                strcpy(title_text, "          AN ODD REGRESSION:");
                set_title_bar(title_text);
                break;
            case 2:
                ;
                strcpy(title_text, "                            ANCIENT JOKES");
                set_title_bar(title_text);
                break;
            case 3:
                ;
                strcpy(title_text, "                                         FIND NEW EXPRESSION");
                set_title_bar(title_text);
                break;
            case 4:
                ;
                strcpy(title_text, "                                                            BURMA-SHAVE.");
                set_title_bar(title_text);
                break;
            }
        }
    } else { // keystroke
        switch(KeyCode) {
        case '2':
            ampm = 0;
            break;
        case 'a':
            ampm = 1;
            break;
        case 'd':
            display_mode = MODE_DATE;
            (void) time(&mode_expires);
            mode_expires += 5;
            break;
        case 't':
            display_mode = MODE_TEST;
            (void) time(&mode_expires);
            mode_expires += 5;
            break;
        case 'q':
            stop_clock();
            break;
        }
    }
}

void stop_clock()
{
    end_display();
    exit(0);
}

int main(int argc, char *argv[])
{
    int letter;  // option character

    // loop through all the options; getopt() can handle together or apart
    while ( ( letter = getopt(argc, argv, "alo:vh")) != -1 ) {
        switch (letter) {
        case 'a':
            ampm = 1;
            break;
        case 'l':
            LED  = 1;
            break;
        case 'o':
            offset = atoi(optarg);
            break;
        case 'v':
            version();
            break;
        case 'h':
            usage(argv[0]);
            break;

        case '?':  // unknown flag; fall through to next case
        default:   // shouldn't happen, but Just In Case
            // note that getopt() warns about the unknown flag
            fprintf(stderr, "run \"%s -h\" for help\n", argv[0]);
            exit(1);
        }
    }

    // All the flags have been scanned.
    // "optind" is now the index of the first non-option item

    if (LED) { // set up the fancy display
        char title_text[] = "         Will would like to remind you that time is not real. Buy gold.         ";
        start_display();
        set_title_bar(title_text);
        register_keyhandler(process_key);

        // As opposed to the test buttons, these are 100% needed and very necessary
        set_key_text(0, "  ?");
        set_key_text(1, "  ??");
        set_key_text(2, " ???");
        set_key_text(3, " ????");
        set_key_text(4, "?????");
    }

    /* get the model running */
    start_timer();

    while (1) {
        get_key(); /* wait for a signal */
    }

    /* no return because never reached */
}


/* This function is called when the timer ticks.
 * Then it calls the show() function, which is our View.
 * The Controller talks to the Model and the View.
 *
 * Note we ignore the argument!
 * sigaction() arranges to pass us the signal that caused the function
 * to be called, so you can use one signal handler for multiple signals.
 * But we only catch one signal, so no need to worry about it.
 */
void tick(int sig)
{
    time_t       now;
    struct tm   *dateinfo;  // localtime() returns a pointer, so it
    // allocates space.  We just need a pointer.

    /* get current time into "struct tm" object */
    (void) time(&now);

    if(offset != 0) {
        now += offset; // Offsets time by given seconds
    }

    dateinfo = localtime( &now );

    if(display_mode != MODE_CLOCK && now == mode_expires ) {
        display_mode = MODE_CLOCK;
    }


    /* call View to show the time */
    if (LED)
        ledshow(dateinfo, ampm);
    else
        show(dateinfo, ampm);
}

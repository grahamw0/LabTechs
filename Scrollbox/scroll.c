/* scroll.c -- scrolling display
 *
 * Darren Provine, 20 March 2012
 */

#include <time.h>
#include <stdio.h>

// sleep needs this
#include <unistd.h>

#include "scroll.h"

// debug flag - changed via options
int debug = 0;

const static char rcsid[] =
    "$Id: $";

static char bugaddress[]="grahamw0@students.rowan.edu";

// Mode switching variables
#define MODE_NORMAL 0x01
#define MODE_CLOCK  0x02
#define MODE_TEST   0x03
#define MODE_DATE	0x04

int box_mode = MODE_NORMAL;
time_t mode_expires;

// version -- say which version this is and exit
// (note simple output; you could also print the rcsid too)
void version()
{
    fprintf(stderr, "scroll version 1\n");
    exit(0);
}

// usage -- print brief summary and instructions
void usage(char *progname)
{
    fprintf(stderr, "This program shows a scrolling box.\n");
    fprintf(stderr, "Usage: %s [-dDvh] text\n", progname);
    fprintf(stderr,
            "  -d # : set scroll delay to # milliseconds (default=1000)\n");
    fprintf(stderr,
            "  -D   : turn on debug messages (more Ds = more debugging)\n");
    fprintf(stderr, "  -v   : show version information\n");
    fprintf(stderr, "  -h   : this help message\n");
    fprintf(stderr, "Use quotes for multi-word messages, 'like this'.\n");
    fprintf(stderr, "report bugs to %s \n", bugaddress);
    exit (0);
}

void exit_scroll()
{
    end_display();
    exit(0);
}

// this is the scroll delay.
// It can be changed keys/mouse, so has to be global.
int delay = 1000;

// keyboard and mouse handling
void process_key(keybits KeyCode)
{
    int row, col;
    if ( KeyCode & 0x80 ) { // mouse click
        row = (KeyCode & 0x70) >> 4;
        col = (KeyCode & 0x0f);

        if(row == 0) {
        	switch(col) {
        		case 0:
        			if(!(delay - 100 < 100)) {
                		delay -= 100;
                		start_timer(delay);
	  				}
	  				break;
	  			case 1:
	  				if(!(delay + 100 > 5000)) {
                		delay += 100;
                		start_timer(delay);
            		}
            		break;
            	case 2:
            		box_mode = MODE_CLOCK;
	  				(void) time(&mode_expires);
	  				mode_expires += 5;
	  				break;
	  			case 3:
	  				box_mode = MODE_TEST;
	  				(void) time(&mode_expires);
	  				mode_expires += 5;
	  				break;
	  			case 4:
	  				exit_scroll();
	  				break;
        	}
        }
        else if (row == 1) {
        	switch(col) {
        		case 0:
        			box_mode = MODE_DATE;
	  				(void) time(&mode_expires);
	  				mode_expires += 5;
        			break;
        	}
        }


        /*if (row == 0 && col == 4) { // "off"
            exit_scroll();
        }
	if(row == 0 && col == 0) {  // Faster
	  if(!(delay - 100 < 100)) {
                delay -= 100;
                start_timer(delay);
	  }
	}
	if(row == 0 && col == 1) {  // Slower
	  if(!(delay + 100 > 5000)) {
                delay += 100;
                start_timer(delay);
            }
	}
	if(row == 0 && col == 2) {
	  box_mode = MODE_CLOCK;
	  (void) time(&mode_expires);
	  mode_expires += 5;
	}
	if(row == 0 && col == 3) {
	  box_mode = MODE_TEST;
	  (void) time(&mode_expires);
	  mode_expires += 5;
	}*/

    } else { // keyboard press
        switch(KeyCode) {
        case 'q':
            exit_scroll();
            break;
        case 's':  // Slower
            if(!(delay + 100 > 5000)) {
                delay += 100;
                start_timer(delay);
            }
            break;
        case 'f':  // Faster
            if(!(delay - 100 < 100)) {
                delay -= 100;
                start_timer(delay);
            }
            break;
        case 't':  // Time
        	box_mode = MODE_CLOCK;
	  		(void) time(&mode_expires);
	  		mode_expires += 5;
	  		break;
	  	case 'y':  // Test (y is next to t- no better hotkey)
	  		box_mode = MODE_TEST;
	  		(void) time(&mode_expires);
	  		mode_expires += 5;
	  		break;
	  	case 'd':
	  		box_mode = MODE_DATE;
	  		(void) time(&mode_expires);
	  		mode_expires += 5;
	  		break;
        }
    }
}

int main(int argc, char *argv[])
{
    int   letter; // option char, if any
    // note C does automatic concatenation of long strings
    char  title[81] =
        "                               "
        "grahamw0 was here!"
        "                               ";

    // loop through all the options; getopt() can handle together or apart
    while ( ( letter = getopt(argc, argv, "d:Dvh")) != -1 ) {
        switch (letter) {
        case 'd':
            delay = atoi(optarg);
            break;

        case 'D':
            debug++;
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

    // optind is the first argument after options are processed
    // if there aren't any, the user didn't give a message to scroll
    if (optind == argc) {
        usage(argv[0]);
    }

    // put the information from the command line into the module
    setup(argv[optind]);

    // set up the view
    register_keyhandler(process_key);
    start_display();
    set_title_bar(title);

    // turn on some keys in row 2
    set_key_text(0, "Date");

    // start the model running
    start_timer(delay);

    // wait for the model to signal controller
    while ( 1 ) {
        get_key();
    }

    end_display();

    return 0;
}


/* This is the function the model uses to signal the controller
 * that there's work to do.
 */
void tick(int sig)
{
    char   *segment;

    time_t now;
    (void) time(&now);
    if(box_mode != MODE_NORMAL && now == mode_expires) {
      box_mode = MODE_NORMAL;
    }

    if(box_mode == MODE_CLOCK) {
    	segment = display_string_time();
    } else if (box_mode == MODE_TEST) {
      segment = display_string_test();
    } else if (box_mode == MODE_DATE) {
    	segment = display_string_date();
    } else {
    segment = display_string();
    }
    
    if (debug >= 3) {
        fprintf(stderr, "display_string() returned |%s|\r\n", segment);
        sleep(2);
    }
    /* Send those chars to the view. */
    show(segment);
    get_key();
}

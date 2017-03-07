/* scroll.h -- header for the scroll box
 *
 * Darren Provine, 17 Nov 2012
 */

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "LEDots.h"

/* defined in "scroll.c" */
void tick(int);
#define MODE_NORMAL 0x01
#define MODE_CLOCK 0x02
#define MODE_TEST 0x03
extern int box_mode;

/* defined in "model.c" */
void start_timer(int);
void setup(char *);
char *display_string();

/* defined in "view.c" */
void show(char *);


/* debug flag */

extern int debug;

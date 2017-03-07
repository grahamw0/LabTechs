/* clock.h -- header file for clock program
 *
 * Darren Provine, 17 July 2009
 */

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "LEDisplay.h"

extern int display_mode;

#define  MODE_CLOCK  0x00
#define  MODE_DATE   0x01
#define  MODE_TEST   0x02

/* model prototypes */
void start_timer(void);

/* controller prototypes */
void tick(int);

/* view prototypes */
void show(struct tm *, int);
void ledshow(struct tm *, int);


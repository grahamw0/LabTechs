/* model.c -- Data and Timing for the Scroll Box
 *
 * Darren Provine, 7 August 2011
 */

#include "scroll.h"

/* the message that we're scrolling */
char scrollmessage[256];


/* pad scrollmessage with 9 spaces left and right */
void setup(char *text)
{
    sprintf(scrollmessage, "         %s         ", text);
    if (debug >= 2) {
        fprintf(stderr, "text is: |%s|\r\n", scrollmessage);
        sleep(1);
    }
}


/* declared outside function so pointer is valid when passed */
char viewport[9];  // Actual chars to display, not null terminated
char *display_string()
{

    int i;
    int startpos;
    static int step = 0; // initialization only happens once
    startpos = step;

    if(startpos == (strlen(scrollmessage) - 9)) {
        step = 0;
        startpos = 0;
    }

    for(i = 0; i < 9; i++) {
        viewport[i] = scrollmessage[startpos];
        startpos++;
    }

    if (debug >= 4) {
        fprintf(stderr, "viewport: |%s|\r\n", viewport);
        sleep(1);
    }

    step++;
    return &viewport[0];
}

/* declared outside function so pointer is valid when passed */
char viewportTime[9];  // Actual chars to display, not null terminated
char *display_string_time()
{
    struct tm *tm;
    time_t t;
    t = time(NULL);
    tm = localtime(&t);
    strftime(viewportTime, sizeof(viewportTime), "%H:%M", tm);
    sprintf(viewportTime, "%s    ", viewportTime);
    return &viewportTime[0];
}

char viewportTest[9];
char *display_string_test() {
  strcpy(viewportTest, "888888888");
  return &viewportTest[0];
}

char viewportDate[9];  // Actual chars to display, not null terminated
char *display_string_date()
{
    struct tm *tm;
    time_t t;
    t = time(NULL);
    tm = localtime(&t);
    strftime(viewportDate, sizeof(viewportDate), "%m/%d/%y", tm);
    sprintf(viewportDate, "%s    ", viewportDate);

    return &viewportDate[0];
}


/* Set up an interval timer for our scroll box.
 * This is part of the model; as with the clock, it's what actually
 * measures real time passing.
 * When the interval is over, notify tick(), which is part of the
 * controller for this program.
 */
void start_timer(int delay)
{
    struct itimerval interval; // interval object
    struct sigaction new_action, old_action;  // signal actions

    /* See the manual entry for signal.h(3HEAD) for a list of all
     * signals. */

    // blank out the signal mask
    sigemptyset( &new_action.sa_mask );
    // clear flags (our application is pretty simple)
    new_action.sa_flags = 0;
    // set tick() as the signal handler when we get the timer signal.
    new_action.sa_handler = tick;

    /* sigaction takes a pointer as an argument, so you have to
     * allocate space.  The declaration of new_action() up above
     * has no star - that's the actual object, not just a pointer.
     */
    if ( sigaction(SIGALRM, &new_action, &old_action) == -1 ) {
        perror("Could not set new handler for SIGALRM");
        exit(1);
    }

    /* set interval to the specified delay value. */
    if (delay >= 1000) {
        interval.it_value.tv_sec = delay / 1000; // seconds
        interval.it_value.tv_usec = 0;
        interval.it_interval = interval.it_value;
    } else {
        interval.it_value.tv_sec = 0;
        interval.it_value.tv_usec = delay * 1000; // we need useconds
        interval.it_interval = interval.it_value;
    }

    /* set a timer for the real time clock using the interval object */
    /* NOTE: takes a pointer, so no * in our declaration of "interval */
    setitimer(ITIMER_REAL, &interval, NULL);
}

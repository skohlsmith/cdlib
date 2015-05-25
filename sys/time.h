/*
 * Time-related defines for use with /std/timefuns.c
 */

#ifndef __SYS_TIME__
#define __SYS_TIME__

/*
 * /sys/time.h
 *
 * This file contains a few definitions that make it easy to convert an
 * integer time to a string. Supported macros are:
 *
 * - TIME_FILE
 * - CONVTIME(time)
 * - TIME2STR(time, sig)
 * - TIME2NUM(time)
 */

/*
 * TIME_FILE
 * 
 * The file defining the time converting functions.
 */
#define TIME_FILE      ("/sys/global/time")

/*
 * TIMESTAMP - Simply a shortcut for the ubiquitous ctime(time()).
 */
#define TIMESTAMP      (ctime(time()))

/*
 * Name   : CONVTIME(t)
 * Returns: string
 *
 * This takes a number of seconds 't' and converts it into a verbose string
 * with the number of days, hours, minutes and seconds in 't'. A component
 * that is zero will not be printed.
 *
 * Example: CONVTIME(175417) returns "2 days 43 minutes 37 seconds"
 */
#define CONVTIME(t)    ((string)TIME_FILE->convtime(t))

/*
 * Name   : TIME2NUM(t)
 * Returns: int *
 *
 * This takes a number of seconds 't' and returns an array of four integers
 * with the number of days, hours, minutes and seconds in 't'.
 *
 * Example: TIME2NUM(175417) returns ({ 2, 0, 43, 37 })
 */
#define TIME2NUM(t)    ((int *)TIME_FILE->time2num(t))

/*
 * Name   : TIME2STR(t, s)
 * Returns: string
 *
 * This takes a number of seconds 't' and returns a string descripting
 * that time in the 's' largest non-zero denominations. The names of
 * the time-elements are abbreviated to only one letter.
 *
 * Example: TIME2STR(175417, 0) returns ""                    (strlen =  0)
 *          TIME2STR(175417, 1) returns "2 d"             (strlen != fixed)
 *          TIME2STR(175417, 2) returns "  2 d 43 m"          (strlen = 10)
 *          TIME2STR(175417, 3) returns "  2 d 43 m 37 s"     (strlen = 15)
 *          TIME2STR(175417, 4) returns "  2 d  0 h 43 m 37 s"   (len = 20)
 *
 * Since 's' == 1 does not have any leading spaced, the length is not
 * fixed. You may want to use sprintf() to get correct alignment if you
 * use only one significant time-element in a table.
 */
#define TIME2STR(t, s) ((string)TIME_FILE->time2str((t), (s)))

/*
 * Name   : TIME2FORMAT(t, f)
 * Returns: string
 *
 * Takes a time() value 't' and returns it as string formatted by the format
 * specifier 'f'. The format may be custom built from the following elements:
 *
 *     yyyy - year in four digits (Example: "2001")
 *     yy   - year in two digis (Example: "01", please try to avoid this)
 *     mmm  - month in string (Example: "Sep")
 *     mm   - month in number with prefix 0 (Example: "09")
 *     -m   - month in number in 2 characters (Example: " 9")
 *     m    - month in number without prefix 0 (Example: "9")
 *     ddd  - day of the week in string (Example: "Mon")
 *     dd   - day of the month in number with prefix 0 (Example: "03")
 *     -d   - day of the month in number in 2 characters (Example: " 3")
 *     d    - day of the month in number without prefix 0 (Example: "3")
 *     All other characters are literally copied to the target string.
 *
 * Examples: "d mmm yyyy" yields "3 Sep 2001"
 *           "yyyymmdd" yields "20010903"
 */
#define TIME2FORMAT(t, f) ((string)TIME_FILE->time2format((t), (f)))


#define TIME ("/std/timefuns.c")

#define YEAR       TIME->time_get_int("year")            /* Year    (integer)  */
#define SEASON     TIME->time_get_int("season")          /* Season  (integer)  */
#define WEEK       TIME->time_get_int("week")            /* Week    (integer)  */
#define DAY        TIME->time_get_int("day")             /* Day     (integer)  */
#define HOUR       TIME->time_get_int("hour")            /* Hour    (integer)  */
#define MINUTE     TIME->time_get_int("minute")          /* Minute  (integer)  */
#define SECOND     TIME->time_get_int("second")          /* Second  (integer)  */
#define WEEKDAY    TIME->time_get_int("weekday")         /* Weekday (integer 0-7)*/
#define TIMESTR    TIME->time_get_string("time")         /* Time-String        */
#define DATESTR    TIME->time_get_string("date")         /* Date-String        */
#define SEASONSTR  TIME->time_get_string("season")       /* Season             */
#define WEEKDAYSTR TIME->time_get_description("weekday") /* Weekday-String     */
#define TIMEDESC   TIME->time_get_description("time")    /* Time-Descr         */
#define DATEDESC   TIME->time_get_description("date")    /* Date_Descr         */
#define EXACTTIME  TIME->time_get_description("exacttime") /* hh:mm:ss */


/*
 * 364 days/year
 * 4 seasons
 * 13 weeks/season
 * 7 days per week
 * hours, minutes, seconds are Real Life(tm)-like
 */

#define MUD_SECONDS_PER_SECOND  3

#define DAYS_PER_YEAR           364
#define SECONDS_PER_MINUTE      60
#define MINUTES_PER_HOUR        60
#define HOURS_PER_DAY           24
#define DAYS_PER_WEEK           7
#define WEEKS_PER_SEASON        13
#define DAYS_PER_SEASON         (DAYS_PER_WEEK*WEEKS_PER_SEASON)
#define SEASONS_PER_YEAR        4

#define NIGHT_BEGINS            21
#define NIGHT_ENDS              5

#define S_PER_HOUR              (SECONDS_PER_MINUTE*MINUTES_PER_HOUR)
#define S_PER_DAY               (S_PER_HOUR*HOURS_PER_DAY)
#define S_PER_WEEK              (S_PER_DAY*DAYS_PER_WEEK)
#define S_PER_SEASON            (S_PER_WEEK*WEEKS_PER_SEASON)
#define S_PER_YEAR              (S_PER_SEASON*SEASONS_PER_YEAR)

/* First REAL year */
#define STARTUP_YEAR            1997

/* First MUD year */
#define FIRST_YEAR              964

/* Time() begins at Jan 1, 1970 */
#define START_YEAR              (STARTUP_YEAR-1970)
#define START_SEASON            0
#define START_WEEK              0
#define START_DAY               0
#define START_HOUR              0
#define START_MINUTE            0
#define START_SECOND            0
#define START_TIME              (START_YEAR*S_PER_YEAR)+\
                                (START_SEASON*S_PER_SEASON)+\
                                (START_WEEK*S_PER_WEEK)+\
                                (START_DAY*S_PER_DAY)+\
                                (START_HOUR*S_PER_HOUR)+\
                                (START_MINUTE*SECONDS_PER_MINUTE)+\
                                (START_SECOND)



/* Easy way to figure out if its day or night */
#define is_day() TIME->query_is_day()
#define is_night() TIME->query_is_night()


#endif

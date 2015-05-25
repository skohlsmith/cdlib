/*
 * /config/sys/time2.h
 *
 * This is the mud-specific part of the time.h file. It should be included
 * by /sys/time.h.
 *
 * Time-related defines for use with /std/timefuns.c
 */

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



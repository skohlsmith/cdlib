
inherit "/std/creature";

#include "/config/login/login.h"
#include <stdproperties.h>
#include <const.h>

create_creature()
{
}

public varargs void
create_body(string race, string gender)
{
    string *a;
    int *stats, i;
    a = explode(file_name(this_object()), "#");
    if(!a || sizeof(a) != 2)
	return;

    set_name("#" + a[1]);

    if(gender)
	set_gender(gender == "male" ? G_MALE : G_FEMALE);
    else
	set_gender(random(2));

    if (race)
	set_race_name(race);
    else
	set_race_name( RACES[random(sizeof(RACES))]);

    set_adj(({ query_gender_string(), query_race_name(), "body", "of" }));
/*
    set_short(query_gender_string() + " " +query_race_name() +
	      " with a tag labeled \"" + query_race_name() +
	      " " + query_name() + "\"");
*/
    set_short(query_gender_string() + " " +query_race_name() +
	      " " + query_name());

    set_long(capitalize(query_real_name()) +
	     " has an empty and blank stare.\n");
    set_appearance(random(98) + 1);
	     
    add_prop(LIVE_I_ALWAYSKNOWN, 1);
    /* add_prop(OBJ_I_INVIS, 1); */

    switch (query_race_name())
    {
    case "human":
	         /* str, dex, con, int, wis, dis */
	stats = ({  9,   9,   9,   9,   9,   9 });
	break;
    case "vogon":
	         /* str, dex, con, int, wis, dis */
	stats = ({  10,   20,   30,  1,  1,  1 });
	break;
    default:
	         /* str, dex, con, int, wis, dis */
	stats = ({  9,   9,   9,   9,   9,   9 });
    }
    for(i = 0; i < 6; i++)
	stats[random(6)]++;

    set_stats(stats);
}

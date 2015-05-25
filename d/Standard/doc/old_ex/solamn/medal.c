/* The Paladine medal for the Solamnian Knight guild. */

inherit "/std/object";

#include "/d/Krynn/common/defs.h"
#include "../guild.h"
#include <stdproperties.h>
#include <ss_types.h>
#include <macros.h>

int knight_level; /* The level of the knight */

void
create_object()
{
    if (!IS_CLONE)
	return;

    knight_level = 0;
    set_name("medal");
    add_name("pal_med");
    set_short("medal of Paladine");
    set_long("This is the Paladine medal, worn by all Solamnian Knights.\n" +
	"For some information about the medal try 'help guild help'.\n");

    add_prop(OBJ_I_VALUE, 0);
    add_prop(OBJ_I_NO_DROP, 1);
    add_prop(OBJ_I_WEIGHT, 0);
    add_prop(OBJ_I_VOLUME, 0);
}

string
query_auto_load() { return MASTER_OB(this_object()) + ":" + knight_level; }

void
init_arg(string str)
{
    if (sscanf(str, "%d", knight_level) != 1)
	knight_level = 0;
}

void
init()
{
    if ((!MEMBER(TP) && !LAYMAN(TP)) && (environment(TO) == TP))
    {
	write("You are not a member of the Solamnian Knights. The medal\n" +
	    "will self destruct.\n");
	remove_object();
    }
    if (!MEMBER(TP) && !LAYMAN(TP))
	return;

    seteuid(getuid(TO));
/* Here I check if the player is being banished */
    if (IN_B_LIST(TP->query_real_name(), 1)) 
	return;

    call_out("soul", 1, TP);
    call_out("set_tax", 1, 0);
}

void
soul(object ob)
{
/* Add a command soul to the player and activate its commands. */
    seteuid(getuid(TO));
    ob->add_cmdsoul(KNIGHT_SOUL);
    ob->update_hooks();
}

void set_knight_level(int level) { knight_level = level; }

int query_knight_level() { return knight_level; }

void
set_tax(int i)
{
    int stat, old_tax;
    object ob;

/* I have different taxes for different levels */

    ob = E(TO);

    old_tax = ob->query_learn_pref(SS_OCCUP);
    if (TAX[knight_level] < old_tax)
	ob->set_guild_pref(SS_OCCUP, old_tax - i);
    else
	ob->set_guild_pref(SS_OCCUP, TAX[knight_level]);
}



/* This file is intended to have some nice combat help functions. */

#include "/d/Krynn/common/defs.h"
#include "/d/Krynn/solace/guild/guild.h"
#include <macros.h>
#include <stdproperties.h>
#include <formulas.h>
#include <filter_funs.h>

#define KNIGHT_I_KICK	"k_knight_i_kick"
#define KNIGHT_I_BASH	"k_knight_i_bash"

/*
 * Tests if and object can attack another
 */
string
c_can_attack(object ob, string verb)
{
    if (!ob || !living(ob) || (ob == TP))
        return C(verb) + " who?\n";

    if (ob->query_prop(OBJ_I_NO_ATTACK))
        return "You sence a devine presence protecting the intended victim.\n";

    if (E(ob) && E(ob)->query_prop(ROOM_I_NO_ATTACK))
        return "You sence that someone has laid a peace spell over " +
			"this room.\n";

    if (!TP->query_npc() && TP->query_met(ob) &&
        TP->query_prop(LIVE_O_LAST_KILL) != ob)
    {
        TP->catch_msg("Attack " + QTNAME(ob) +
                                 "?!? Please confirm by trying again.\n");
        TP->add_prop(LIVE_O_LAST_KILL, ob);
        return "";
    }

    if (!F_DARE_ATTACK(TP, ob))
        return "You don't dare to attack.\n";
}

/*
 * Function name: tell_watcher
 * Description:   Send the string from the fight to people that want them
 * Arguments:     The string to send
 */
static void
tell_watcher(string str, object me, object enemy)
{
    object *ob;
    int i;

    ob = FILTER_LIVE(all_inventory(environment(me))) - ({ me });
    ob -= ({ enemy });
    for (i = 0; i < sizeof(ob); i++)
        if (ob[i]->query_see_blood())
            ob[i]->catch_msg(str);
}



inherit "/std/object.c";

#include <stdproperties.h>
#include <macros.h>
#include <time.h>

void create_object()
{
 set_name(({"watch", "digital watch"}));
 set_short("watch");
 set_long("@@my_long");

 add_prop(OBJ_M_NO_DROP, "@@did_drop");
}

string my_long()
{
 /* using catch_msg()'s ({ met_msg, unmet_msg, unseen_mesg }) functionality */
 say(({QCTNAME(this_player()) + " glances at " + this_player()->query_possessive() + " watch.\n",
       QCTNAME(this_player()) + " glances at " + this_player()->query_possessive() + " watch.\n",
       ""}));
 return "A glance at your watch tells you that it is " +EXACTTIME+ " on " +DATEDESC+ ".\n";
}

void did_drop()
{
 set_alarm(1.0, 0.0, "do_drop_remove");
}

void do_drop_remove()
{
 if(environment(this_object())->query_prop(ROOM_I_IS))
 {
  tell_room(environment(this_object()),
   "The watch obliterates itself upon touching the ground.\n");
  this_object()->remove_object();
 }
}


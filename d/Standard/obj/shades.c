inherit "/std/object";
inherit "/lib/wearable_item";

#include <stdproperties.h>
#include <macros.h>
#include <composite.h>
#include <wa_types.h>

void create_object()
{
 set_name(({"shades", "glasses"}));
 add_adj(({"sun", "black", "wraparound", "pair", "of"}));
 set_short("pair of shades");
 set_long("A pair of black wraparound shades.\n");
 add_prop(OBJ_S_WIZINFO, "These let you see in the dark.\n");
 config_wearable_item(A_EYES, 1, 0, 0);		// 1 layer, no layering allowed, we don't define wear() and remove()
}

// Tie into the /cmd/live/items command for 'wear'
mixed command_wear()
{
 mixed retval;

 retval = wear_me();
 if(intp(retval) && retval == 1)	// it's not a fail message, and did succeed
 {
  wearer->add_prop(LIVE_I_SEE_DARK, 3);
 }

 return retval;
}

// Tie into the /cmd/live/items command for 'remove'
mixed command_remove()
{
 mixed retval;
 object p;

 p = wearer;

 retval =  remove_me();
 if(intp(retval) && retval == 1)	// remove succeeded
 {
  if(p)		// ...and the saved player object is still valid
  {
   // What if the player has multiple objects granting LIVE_I_SEE_DARK?
   p->remove_prop(LIVE_I_SEE_DARK);
  }
 }

 return retval;
}


void leave_env(object from, object to)
{
 // What if the player has multiple items granting LIVE_I_SEE_DARK?
 from->remove_prop(LIVE_I_SEE_DARK);

 ::leave_env(from, to);
 wearable_item_leave_env(from, to);
}

void appraise_object(int num)
{
 ::appraise_object(num);
 appraise_wearable_item();
}

// This object stays with the player across gaming sessions
string query_auto_load()
{
 // wearable_item defines the 'worn' variable
 add_prop(OBJ_M_NO_DROP, 1);
 set_alarm(0.1, 0.0, &remove_prop(OBJ_M_NO_DROP));
 return "/d/Standard/obj/shades:" + worn;
}

void init_arg(string str)
{
 int isworn;

 sscanf(str, "%d", isworn);

 if(isworn)
   set_alarm(1.0, 0.0, "do_auto_wear");
}

void do_auto_wear()
{
 command_wear();
}

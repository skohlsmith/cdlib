/* skills.c
   Commander / Tintin 920118 */

inherit "/std/room";

#include <stdproperties.h>
#include <std.h>
#include <macros.h>
#include "/config/login/login.h"

/*
 * Function name: create_room()
 * Description  : Create the room.
 * In this function, we declare everything that should be fixed
 * when the room is started, and not changed in resets.
 */

create_room()
{
    set_short("skills room");
    set_long("\nYou are in an a small room. In the room there is a\n" +
	     "busy looking robot. In a corner you can see a teleporter.\n");
    add_item( ({"robot" }),
	     "The robot has lots of technical equipment. The robot has a note\n" +
	     "saying: Simply 'ask' me if you need help.\n");
    add_prop(ROOM_I_INSIDE, 1);
}

/*
 * Function name: init
 * Description  : Add commands etc.
 */
public void
init()
{
    object ob;
    add_action("all_cmd", "", 1);
    if (this_player()->query_ghost() & GP_SKILLS)
    { 
	ob = clone_object(PATH + "set_skills");
	ob->move(this_player());
    } 
}

/*
 * Function name: long
 * Description  : Modify the behaviour of long
 */
public varargs mixed
long(string arg)
{
    mixed str;

    str = ::long(arg);

    return str;
}

/*
 * Function name: enter_cmd
 * Description  : Catch all player commands. Take care of the legal ones
 *                and throw all the others away
 */
public void 
enter_cmd(string str)
{
    object player, *a;
    string tmp;
    
    if (!str || !strlen(str))
    {
	write("Enter what?\n");
	return;
    }
    player = this_interactive();
    if (str == "teleporter" || str == "hatch")
    {
	if (player->query_ghost() & GP_SKILLS)
	{
	    write("You can't force your legs to move towards the teleporter.\n");
	    return;
	}
	else if (MASTER_OB(player) != LOGIN_NEW_PLAYER)
	{
	    write("You do not belong here, so you do right to leave.\n");
	    player->move_living("through teleport", player->query_def_start());
	    return;
	}
	else
	{
	    write("As you enter the teleporter, there is a flash of light and\n" +
		  "you suddenly find yourself in a very different place.\n\n");
	    player->set_ghost(0);
	    player->ghost_ready();
	    return;
	}
	return;
    }
    write("You find no " + str + " to enter.\n");

    return;
}

void leave_inv(object ob, object to)
{
    object t;
    ::leave_inv(ob, to);
    if (t = present("trainer",ob))
    {
	t->remove_object();
	if (t)
	    "/secure/master"->do_debug("destroy", t);
    }
}

/*
 * Function name: all_cmd
 * Description  : Catch all player commands. Take care of the legal ones
 *                and throw all the others away
 */
public int
all_cmd(string str)
{
    switch(query_verb())
    {
    case "arrive":
    case "glance":
    case "depart":
    case "quit":
    case "Goto":
    case "goto":
    case "look":
    case "exa":
    case "bug":
    case "typo":
    case "praise":
    case "idea":
    case "sysbug":
    case "i":
    case "inv":
    case "examine":
	return 0;
	break;
    case "enter":
	enter_cmd(str);
	return 1;
	break;
    case "help":
	write("You are supposed to select a knowledge chip here. Do:\n");
	write("     'instructions'\n");
	write("This will give you further instructions.\n");
	return 1;
    case "say":
	this_player()->command(str);
	return 0;
    default:
	if (this_interactive()->query_wiz_level() > 40)
	    return 0;
	write("The robot looks like it doesn't understand you.\n");
	return 1;
    }
}

/* features.c
   Tintin 920106 */

inherit "/std/room";

#include <stdproperties.h>
#include "/config/login/login.h"

/*
 * Function name: create_room()
 * Description  : Create the room.
 * In this function, we declare everything that should be fixed
 * when the room is started, and not changed in resets.
 */

create_room()
{
    object ob;

    set_short("feature room");
    set_long("You are in an a small room. In the middle of the room\n" +
	     "there is some large machinery. How about taking a look\n" +
	     "at it?\n");

    add_item( ({"machine", "machinery" }),
	     "This machine has three dials labled alpha, beta and gamma.\n" +
	     "There is also a green button. Above there is a sign saying:\n" +
	     "Set the dialals to preferred radiation. Then press the button.\n" +
	     "The dials can be set to 0 through 5. Example 'beta 3'.\n");

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
    if (this_player()->query_ghost() & GP_FEATURES)
    {
	ob = clone_object(PATH + "set_attrib");
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
    mixed str, *a;

    str = ::long(arg);

    if (arg)
	return str;

    a = this_player()->query_adj(1);
    if (sizeof(a) < 2 || a[0] == "" || a[1] == "")
    	str += "\nYou are still rather featureless. " +
	    "Using the machine might help.\n";
    else
	str += "\nYou suspect that you can do nothing more here.\n" +
	    "Maybe you should leave through the teleporter.\n";

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
    object player, ob;
    string tmp, *a;

    if (!str || !strlen(str))
    {
	write("Enter what?\n");
	return;
    }
    player = this_player();
    if (str == "teleporter")
    {
	a = player->query_adj(1);
	if (sizeof(a) < 2 || a[0] == "" || a[1] == "")
	    write("An invisible force prevents you from leaving.\n");
	else if (!player->query_ghost())
	    player->move_living(" through the teleporter.",
				player->query_def_start());
	else
	{
	    if (ob = present("selector", player))
		ob->remove_object();
	    write(break_string
		  ("As you enter the teleporter, there is a flash of light " +
		   "and you suddenly find yourself in a very different " +
		   "place.", 78));
	    player->set_ghost(player->query_ghost() - GP_FEATURES);
	    if (!(player->query_ghost() & GP_SKILLS))
	    {
		player->set_ghost(0);
		player->ghost_ready();
		player->move_living(" through the teleporter.",
				    player->query_def_start());
	    }
	    else
		player->move_living(" through the teleporter.", PATH + "skills");
	}
	return;
    }
    write("You find no " + str + " to enter.\n");

    return;
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
    case "password":
    case "glance":
    case "depart":
    case "say":
    case "quit":
    case "Goto":
    case "goto":
    case "look":
    case "exa":
    case "typo":
    case "bug":
    case "idea":
    case "sysbug":
    case "praise":
    case "examine":
	return 0;
	break;
    case "enter":
	enter_cmd(str);
	return 1;
	break;
    case "help":
	write("You are supposed to select your main personal features. Do:\n");
	write("     'read hologram'\n");
	write("This will give you further instructions.\n");
	write("When you are done, you can do 'enter teleporter'\n");
	return 1;
    case "leave":
    case "exit":
	enter_cmd(str ? str : "teleporter");
	return 1;
    default:
	if (this_interactive()->query_wiz_level() > 40)
	    return 0;
	write("That is not possible here.\n");
	return 1;
    }
}


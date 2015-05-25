/* mangle.c
   Tintin 920106 */

inherit "/std/room";

#include <stdproperties.h>
#include <macros.h>
#include "/config/login/login.h" 

int length, width;
mixed *pos;
int *proc;

/*
 * Prototypes
 */
public void mangle_player(object pl, int l, int w);

/*
 * Function name: create_room
 * Description  : Create the room.
 * In this function, we declare everything that should be fixed
 * when the room is started, and not changed in resets.
 */
void
create_room()
{

    set_short("mangle room");
    set_long("You are in an a large room. One side of the room is\n" +
	     "occupied by an enormous mangle with a conveyor belt leading\n" +
	     "into the interior of the machine. On the wall\n" +
	     "by the machine there are two large touchdials. One is marked\n" +
	     "LENGTH and has the positions 'very short', 'short',\n" +
	     "'normal', 'tall', 'very tall' and 'random'. The other one\n" +
	     "is marked WIDTH and has the settings 'skinny', 'lean',\n" +
	     "'normal', 'plump', 'fat' and 'random'.\n");
    pos = ({
	({"very short", "short", "normal", "tall", "very tall",  "random"}),
	({"skinny", "lean", "normal", "plump", "fat" , "random"}),
	});
    proc = ({ 60, 80, 100, 120, 140 });
    add_item( ({ "dial", "dials",}),
	     "@@show_dials");
    add_item( ({ "machine", "mangle" }),
	     "The mangle is a huge contraption with a titan-iridium frame\n" +
	     "and lots of belts, magnets and electronics inside.\n");
    add_prop(ROOM_I_INSIDE, 1);
}

public string
show_dials()
{
    return 
	"The LENGTH dial is set to " + pos[0][length] + ".\n" +
	    "The WIDTH dial is set to " + pos[1][width] + ".\n";
}
    

/*
 * Function name: init
 * Description  : Add commands etc.
 */
public void
init()
{
    add_action("all_cmd", "", 1);
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

    if (arg)
	return str;

    str += "\n" + show_dials();

    if (this_player()->query_ghost() & GP_MANGLE)
	str += "\nYou are still rather shapeless. Something that the\n" +
	    "machine in front of you could change.\n";
    else if (!this_player()->query_ghost())
	str += "\nYou get a strong feeling that this is not a place for\n" +
	    "the living. You should probably leave as fast as you can. You\n" +
            "can see a conveyor belt by the side of the machine that could\n" +
            "take you out of here.\n";
    else 
	str += "\nWell, get going! You have your features left to fix.\n";
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
    if (!str)
    {
	write("Enter what?\n");
	return;
    }

    switch (str)
    {
    case "belt":
    case "conveyor":
    case "conveyor belt":
	if(this_interactive()->query_ghost())
	{
	    write("Enter what?\n");
	    return;
	}
	this_interactive()->move_living("onto the conveyor belt", 
					this_interactive()->query_def_start());
	break;
	
    case "mangle":
    case "machine":
	/* This is where the player gets modified. Awfully hardcoded
	    Im afraid
	 */
	mangle_player(this_interactive(), length, width); 
	break;
    default:
	write("Enter what?\n");
	break;
    }
}

public void
turn_cmd(string str)
{
    string knob, set;
    int i;

    if (!str)
    {
	write(capitalize(query_verb()) + " what?\n");
	return;
    }
    if (!parse_command(str, ({ }),
		       "[dial] %w [dial] [to] %s",
		       knob, set))
    {
	write(capitalize(query_verb()) + " what?\n");
	return;
    }

    /* Yes, this is ugly, I know */
    switch (lower_case(knob))
    {
    case "length":
	i = member_array(set, pos[0]);
	if (i == (sizeof(pos[0]) - 1))
	    length = random(sizeof(pos[0]) -1);
	else if (i >= 0)
	    length = i;
	else
	{
	    write("What value do you want to set it to?\n");
	    return;
	}
	break;
    case "width":
	i = member_array(set, pos[1]);
	if (i == (sizeof(pos[1]) - 1))
	    length = random(sizeof(pos[1]) -1);
	else if( i >= 0)
	    width = i;
	else
	{
	    write("What value do you want to set it to?\n");
	    return;
	}
	break;
    default:
	write("Which dial do you want to set?\n");
	return;
    }
    write("You set the " + knob + " dial to " + set + ".\n");
    say(break_string(QCTNAME(this_interactive()) +
		     " sets the " + knob + " to " + set + ".\n",76));
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
    case "depart":
	say(QCTNAME(this_interactive()) + " enters the mangle.\n");
	return 1;
	break;
    case "arrive":
    case "glance":
    case "say":
    case "quit":
    case "Goto":
    case "goto":
    case "look":
    case "bug":
    case "typo":
    case "idea":
    case "praise":
    case "sysbug":
    case "exa":
    case "examine":
    case "who":
	return 0;
	break;
    case "enter":
	enter_cmd(str);
	return 1;
	break;
    case "set":
    case "touch":
	turn_cmd(str);
	return 1;
	break;
    case "help":
	write("You are supposed to mangle your body into shape.\n");
	write("You set the dials with for example:\n");
	write("   'set dial length short' or 'touch dial width to random'\n");
	write("When you have done so, you enter the mangle with:\n");
	write("          'enter mangle'\n");
	return 1;
	break;
    default:
	if (this_interactive()->query_wiz_level() > 40)
            return 0;
	write("That is not possible here.\n");
	return 1;
    }
}

/*
 * Function name: mangle_player
 * Description:   Mangle a player into shape and move to the next room
 * Arguments:     pl: player to mangle
 */
public void 
mangle_player(object pl, int l, int w)
{
    int myl, myw;
    int *attr;

    if (!(pl->query_ghost() & GP_MANGLE))
	return;

    set_this_player(pl);

    write("You enter the mangle. You can feel it starting to shape your\n");
    write("shapeless body. Strangely enough, it is not painful...\n");

    attr = RACEATTR[pl->query_race_name()];

    if (!sizeof(attr))
    {
	write("You have a bad body, go get a better one.\n");
	pl->set_ghost(pl->query_ghost() | GP_BODY);
	pl->ghost_start();
    }

    myl = (attr[0] * proc[l]) / 100;
    myw = (attr[1] * proc[l]) / 100;
    
    pl->add_prop(CONT_I_HEIGHT, myl);
    pl->add_prop(CONT_I_WEIGHT, myw);
    
    write("...you are slowly passed through the mangle and after awhile\n");
    write("you come out of it as a new " + pl->query_race_name() + ".\n\n");

    pl->set_ghost(pl->query_ghost() & ~GP_MANGLE);

    if (pl->query_ghost() & GP_FEATURES)
	pl->move_living("X", PATH + "features");
    else if (pl->query_ghost() & GP_SKILLS)
	pl->move_living("X", PATH + "skills");
    else
    {
	pl->set_ghost(0);
	pl->ghost_ready();
    }
}

 

/* death_room.c */
/* Mrpr 901119 */
/* Tintin 911031 */

inherit "std/room";

#include "/sys/stdproperties.h"

#define DEATH_ROOM "/d/Standard/death/death_room"
#define WRITE(x) tell_object(players[j], x)

mixed players;

/*
 * Prototypes
 */
void create_death();
void add_player(object plobj);
void do_remove();
void remove_death_obj(object player);
void remove_player(object plobj);

void
init()
{
    
    ::init();
    
    add_action("filter", "", 1);
    
    if(!this_player()->query_ghost())
	{
	    write("The doctor says: Take two asprin and call me on Monday.\n");
	    this_player()->move(this_player()->query_default_start_location());
	    return;
	}
	
    add_player(this_player());
    set_heart_beat(1);
}


/*
 * Function name: create_room
 * Description:   Reset the room
 */
void
create_room() 
{
    players = ({ });
    set_short("hospital");
    set_long(
"A typical room in a hospital. There are arms and legs all over the place.\n" +
"This looks like the place for unrepareable bodies.\n\n");
}

/*
 * Function name: do_remove
 * Description:   Removes players that's finished
 */
void
do_remove()
{
    int j, nr;
    
    if (!sizeof(players))
	return;
    
    nr = sizeof(players);
    
    for (j = 0 ; j < nr ; j += 2)
    {
	if (players[j + 1] >= 70)
	{
	    if (players[j])
	    {
		remove_death_obj(players[j]);
		players[j]->reincarnate();
	    }
	    players = exclude_array(players, j, j + 1);
	    do_remove();
	    return;
	}
    }
}

/*
 * Function name: remove_death_obj
 * Description:   Remove the "death_mark"-object, add badge.
 */
void
remove_death_obj(object player)
{
    
    object plobj, badge;
    
    plobj = present("death_mark", player);
    while (plobj = present("death_mark", player))
	plobj->remove_object();
}

/*
 * Function name: add_player
 * Description:   Adds a player to the list
 */
void
add_player(object plobj)
{
    players = players + ({plobj, 0});
}

/*
 * Function name: remove_player
 * Description:   Removes a player from the list
 */
void
remove_player(object plobj)
{
    
    int i;
    
    if(!(i = sizeof(players)))
	return;
    
    if (i <= 2)
    {
	if (players[0] == plobj)
	{
	    players = ({ });
	    set_heart_beat(0);
	    return;
	}
	return;
    }

    i = member_array(plobj, players);
    if (i >= 0)
	players = exclude_array(players, i, i + 1);
}

/*
 * Function name: leave_inv
 * Description:   Remove players if they leave the room prematurly
 */
void
leave_inv(object ob, object to)
{
    remove_player(ob);
    ::leave_inv(ob, to);
}

/* 
 * Function name: filter
 * Description:   Filter out relevant commands.
 */
int
filter(string str)
{
    switch(query_verb())
    {
    case "quit":
    	write("The doctor says: Star Burgers will get your body!\n");
	return 0;
    case "look":
    case "take":
    case "get":
	return 0;
    }    
    write("That is impossible in your state.\n");
	return 1;

}

/*
 * Function name: heart_beat
 * Description:   Let the actions be governed by time.
 */
void
heart_beat()
{
    
    int align, j, nr;
    
    if (!(nr = sizeof(players)))
    {
	set_heart_beat(0);
	return;
    }
    
    for (j = 0 ; j < nr ; j += 2)
    {
	players[j + 1]++;
	
	if (players[j + 1] == 5)
	{
	    
	    
	    WRITE("\nThe doctor says: Sorry pal. Your body is beyond repair. \n" +
		  "\nThe doctor shrugs and leaves the room.\n\n");
	    
	}
	
	if (players[j + 1] == 10)
	{
	    
	    WRITE("\nYou feel lonely despite all the body parts around you.\n" +
		  "This body was good, and you are sure you will miss it.\n\n");
	    
	}
	
	if (players[j + 1] == 15)
	{
	    
	    WRITE("\nTwo men enters the room. Without a word they put you on a\n" +
		  "cart and wheel you out of the room and down a corridor.\n\n");
	    
	}
	
	if (players[j + 1] == 20)
	{
	    
	    WRITE(
"\nThey take down endless corridors, up with elevators, through more\n" +
"corridors and finally stop at a desc in a small room. They sign a\n" +
"paper handed to them by a man at a desc. You only manage to see that\n" +
"the top of a paper has a head saying: 'STAR BURGER - The best hamburger\n" +
"in universe!'. When the paper is handed back to the man behind the desc\n" +
"they take you through a hatch and into another room. There they place your\n" +
"body in front of a terminal.\n\n");
	}
    }
	
    do_remove();
    
}


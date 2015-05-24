/* death_mark.c */
/* Mrpr 901122 */
/* Tintin 911031 */

#include <stdproperties.h>
#include <macros.h>

#define DEATH_ROOM "/d/Standard/death/death_room"

inherit "/std/object";

/*
 * Function name: create_object
 * Description:   Create the deathmark
 */
static void
create_object()
{
    add_prop(OBJ_I_WEIGHT, 0); /* 0 g */
    add_prop(OBJ_I_VOLUME, 0); /* 0 ml */
    add_prop(OBJ_I_VALUE, 0); /* 0 copper */
    add_prop(OBJ_I_NO_DROP, 1); /* Call drop when property tested */
    add_prop(OBJ_I_NO_GIVE, 1); /* Call give when property tested */
    set_name("death_mark");
    set_no_show();
}

/*
 * Function name: init
 * Description:   Init this object
 */
init()
{
    start_death();
}

/*
 * Function name: start_death
 * Description:   Start the death sequence.
 */
void
start_death()
{
    object ned, my_host;
    
    my_host = environment(this_object());
    
    if (!my_host || !living(my_host) || !my_host->query_ghost())
    {
	remove_object();
	return;
    }

    
    write("A ambulance helicopter arrives and some men dressid in white\n" +
"come out of it. They collect what is left of your body and put it on a\n" +
"stretcher and in a couple of plastic bags. Then they put you in the\n" +
"helicopter. You faint.\n");
    
    write("You find yourself in another place.\n\n");
    my_host->move(DEATH_ROOM, 1);

}

/*
 * Function name: query_auto_load
 * Description:   Automatic load of this object
 */
query_auto_load()
{
    return MASTER_OB(this_object());
}


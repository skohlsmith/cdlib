/*
 *   The storage room of the example shop
 */

inherit "/std/room";

#include <stdproperties.h>

create_room()
{
   set_short("The shop's store room");
   set_long("This is the store room for the shop.\n");

   add_exit("/d/Genesis/doc/examples/trade/shop", "south");

   add_prop(ROOM_I_INSIDE, 1);  /* This is an inside room */
}

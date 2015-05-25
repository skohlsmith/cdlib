/*

   /std/room/map.c

   Room with handling of terrain exits.
   ----------------------------------------------------------------------
   Defined functions and variables:


   ----------------------------------------------------------------------
*/

#include "/sys/map.h"
#include "/secure/std.h"

/*
 * Function name:   map_file
 * Description:     Returns a filename for a given coordinate on the map.
 *                  This is always a master file, never cloned instances.
 *                  New rooms are actually created with write_file so as to
 *                  know which rooms are there to go to.
 *                  This function is used as 'value by function call' parameter
 *		    in add_exit: add_exit("@@map_file|xnnnn.mmynnnn.mm",
 *		    "north",0);
 * Arguments:       xyc: World coordinates "xnnnn.mmynnnn.mm"
 * Returns:         Filename
 */
public string
map_file(mixed xyc)
{
    string base_file;
    int wlx, wly, slx, sly;


    if ((pointerp(xyc)) && (sizeof(xyc) > 0))
	xyc = xyc[0];

    if (find_object(MAP_PATH + "/" + xyc))
	return MAP_PATH + "/" + xyc;


    if (sscanf(xyc, "x%d.%dy%d.%d", wlx, slx, wly, sly) != 4)
	return MAP_DEFAULT_ROOM;


    if ((slx < 0) || (slx > MAP_SL_MAX) || (sly < 0) || (sly > MAP_SL_MAX))
	return MAP_DEFAULT_ROOM;

    base_file = (string) SECURITY->find_maplink(xyc);

    if (!base_file)
	return MAP_DEFAULT_ROOM;

    if (SECURITY->create_maproom(xyc,
			     "inherit \"" + base_file + "\";\n" +
			     "create_mapsquare(int wlx, int wly, int slx, int sly) { ::create_mapsquare(wlx, wly, slx, sly); }\n"))
	return MAP_PATH + "/" + xyc;
    else
	return MAP_DEFAULT_ROOM;
}



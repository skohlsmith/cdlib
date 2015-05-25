/*
   map/manager.c

   Ver 1.0

   This file should be inherited by the domains map manager,
   see 'man maplinks'.

*/
#include "/secure/std.h"
#include <macros.h>
#include <map.h>

static	mapping	terrain_files;
static	string	*overlap_map;
static	int	overlap_exists, overlap_x, overlap_y; /* Bottom left corner */
static	mapping	single_locs;

void create()
{
    setuid();
    seteuid(getuid(this_object()));
    single_locs = ([]);
    overlap_map = ({});
    terrain_files = ([]);
    this_object()->create_map_manager();
}

/*
 * Function name:   add_single
 * Description:     Adds one maplocation as managed by the domain
 * Arguments:       wml: WL maplocation coordinate as a string, "xnnnnynnnn"
 *                       Example: "x123y456"
 *		    tfile: The terrainfile to use for this specific wml
 */
void
add_single(string wml, string tfile)
{
    single_locs[wml] = tfile;
}

/*
 * Function name:   set_box
 * Description:     Sets the 'box' of coordinates that are to be managed
 *		    by the domain.
 * Arguments:       xbottom: Lowerleft x coordinate in map of box
 *		    ybottom: Lowerleft y coordinate in map of box
 *		    mapbox: Array of strings with one char / maplocation
 *		    terrains: mapping of terrainfiles corresponding to
 *		              the single chars in mapbox
 * Example:         xbottom = 100; ybottom = 100;
 *		    mapbox = ({
 *				".n.."
 *				".nn."
 *				".n.."
 *			     });
 *                  terrains = ([
 *				  "n" : "/d/Standard/terrain/hills"
 *				  "." : "/d/Standard/terrain/water"
 *				]);
 */
void
set_box(int xbottom, int ybottom, string *mapbox, mapping terrains)
{
    overlap_x = xbottom;
    overlap_y = ybottom;
    overlap_exists = 1;
    overlap_map = mapbox + ({});
    terrain_files = terrains;
}

/*
 * Function name:   find_maplink
 * Description:     Finds out what terrain file linked to a given
 *                  maplocation. The entire maplocation must have
 *                  the same terrainfile.
 * Arguments:       wlc: WL coordinate as a string, "xnnnn.mynnnn.m"
 *                       Example: "x123.4y45.6"
 * Returns:         Filename of the file to instance for this WL
 *		    or 0 if default terrainfile should be used.
 */
string find_maplink(string wlc)
{
    int xc, yc, sym;
    string ter;

    if (sscanf(wlc, "x%d.%dy%d.%d", xc, sym, yc, sym) != 4)
	return 0;

    ter = single_locs["x" + xc + "y" + yc];
    if (stringp(ter))
	return ter;

    if (!sizeof(overlap_map) || !mappingp(terrain_files) || !overlap_exists)
	return 0;

    xc -= overlap_x;
    yc -= overlap_y;
    if (yc < 0 || xc < 0)
	return 0;

    yc = sizeof(overlap_map) - yc - 1;
    if (yc < 0)
	return 0;

    if (xc >= strlen(overlap_map[yc]))
	return 0;

    ter = overlap_map[yc][xc..xc];

    return terrain_files[ter];
}

/*
 * Function name:   make_link
 * Description:     Sets up the link for this domain with /secure/master
 *		    This is called from within /secure/master if this object
 *		    is denoted as the map manager of the domain.
 */
void
make_link()
{
    string box;

    if (overlap_exists && sizeof(overlap_map))
    {
	box = "x" + overlap_x + "-" +
	    (overlap_x + strlen(overlap_map[0]) - 1) +
	    "y" + overlap_y + "-" +
	    (overlap_y + sizeof(overlap_map) - 1);
    }
    else
	box = 0;

    SECURITY->add_maplink(MASTER, box, m_indexes(single_locs));
}

/*
 * Function name:   save_link
 * Description:     Notifies this file as the map manager of the domain
 *
 */
void save_link()
{
    SECURITY->set_map_manager(MASTER);
}

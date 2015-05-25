/*
   /std/mapsquare.c

   This file is used as the standard file to inherit in mapped rooms

   JnA 910720
*/
#pragma save_binary

inherit "std/room";

#include "/sys/map.h"
#include "/secure/std.h"
#include <stdproperties.h>

#define MAP_OBJ "/std/map/map"

#include "/std/mapsquare/paths.c"
#include "/std/mapsquare/roads.c"
#include "/std/mapsquare/rivers.c"

/*
 * Global Variables
 */

static string *exceptions; /* ({ "x.mmy.mm", file }) */
static string redirect;
static int    random_encounter;

/* Forward Declarations */

void init_exits(int wlx, int wly, int slx, int sly);

/* Simple set and Query Functions */

void set_exceptions(string *exc) { exceptions = exc; }
void add_exceptions(string *exc) { exceptions += exc; }
string *query_exceptions()       { return exceptions; }

void
create_mapsquare(int wlx, int wly, int slx, int sly)
{
    set_short("Matrix");
    set_long("This is an empty and flat terrain.\n");
    set_noshow_obvious(1);

    init_exits(wlx, wly, slx, sly);
}

void
create_room()
{
   int wlx, wly, slx, sly, il;
   string xyc, d_xyc, slc;

   SECURITY->remove_maproom();  /* Remove the file of this room */

   sscanf(file_name(this_object()), MAP_PATH + "/%s", xyc);

   if (sscanf(xyc, "x%d.%dy%d.%d", wlx, slx, wly, sly) != 4)
   {
       call_out("remove_object", 1);
       return 0;
   }

   add_prop(OBJ_S_WIZINFO, "@@print_wizinfo@@");

   il = SECURITY->find_remote_map_sym(xyc);
   if (il & RANDOM_ENCOUNTER)
       random_encounter = 1;
   else
       random_encounter = 0;

   create_mapsquare(wlx, wly, slx, sly);

   slc = "x." + slx + "y." + sly;

   il = member_array(slc, exceptions);
   if (il >= 0)
   {
       redirect = exceptions[il+1];
       return;
   }
   else
       redirect = 0;

   call_out("check_empty", 150);
}

void
check_empty()
{
    if (!sizeof(all_inventory(this_object())))
	call_out("remove_object", 30);
}

void
leave_inv(object ob, object to)
{
    check_empty();
    ::leave_inv(ob, to);
}

void
enter_inv(object ob, object from)
{
    if (redirect)
    {
	ob->move(redirect, 1);
	ob->move(redirect, 0);
    }
    remove_call_out("remove_object");
    remove_call_out("check_empty");
    ::enter_inv(ob, from);
}

/*
 * Find out if a random encounter should happen in this WL
 */
public int
query_random_encounter()
{
    return random_encounter;
}

static string
add_coord(int wl, int sl, int delta)
{
    sl += delta;

    if (sl < 0)
	return "" + (wl-1) + "." + MAP_SL_MAX;

    if (sl > MAP_SL_MAX)
	return "" + (wl + 1) + ".0";

	return "" + wl + "." + sl;
}

static void
check_dest(string my_xyc, string to_xyc)
{
    int mwlx, mwly, twlx, twly, tslx, tsly, dummy;

    sscanf(my_xyc, "x%d.%dy%d.%d", mwlx, dummy, mwly, dummy);
    sscanf(to_xyc, "x%d.%dy%d.%d", twlx, tslx, twly, tsly);

    if ((mwlx == twlx) && (mwly = twly)) return;

    /* We need to find neighbour mapchar and store it.
       Fix when we get an external database
    */
    return;
}


#define DIRS ({ "north", "south", "east", "west", "northeast", "northwest", "southeast", "southwest" })

#define DIRS_XDELTA ({ 0, 0, 1, -1, 1, -1, 1, -1 })
#define DIRS_YDELTA ({ 1, -1, 0, 0, 1, 1, -1, -1 })

void
init_exits(int wlx, int wly, int slx, int sly)
{
    int il;
    string xyc, d_xyc;

    xyc = "x" + wlx + "." + slx + "y" + wly + "." + sly;
    for (il=0; il<sizeof(DIRS); il++)
    {
	d_xyc = "x" + add_coord(wlx, slx, DIRS_XDELTA[il]) +
	    "y" + add_coord(wly, sly, DIRS_YDELTA[il]);
	check_dest(xyc, d_xyc);
	add_exit("@@map_file|"+d_xyc, DIRS[il], 0);
    }
}

static string *
add_path(string *list, string new)
{
    if (member_array(new, list) < 0)
	return list + ({ new });
    else
	return list;
}

/*
 * Function name: query_dirs_here
 * Description:   Gives the roadpaths in a given WL.SL square.
 * Arguments:     wlx: World coordinate X.
 *                wly: World coordinate Y.
 *                slx: Square coordinate X.
 *                sly: Square coordinate Y.
 *		  mlnet: The network of paths, rivers, or roads in the ML
 *		  ({
 *		    ({ sl_nn, sl_nn, sl_nn, sl_nn, })
 *		    ({ sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, })
 *		    ({ sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, })
 *		    ({ sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, })
 *		  })
 * Returns:       Directions as an array.
 *                ({ "nw", "n", "ne", "e", "se", "s", "sw", "w" })
 *                Any or none can be included.
 */
public string *
query_dirs_here(int wlx, int wly, int slx, int sly, mixed mlnet)
{
  string *localpaths;
  int il, mynum, pos;
  mapping dir_map;

  localpaths = ({});
  mynum = MAP_SL(slx, sly);
  dir_map = mkmapping(MAP_SL_DIFFS, MAP_SL_DIRS);

  for (il = 0; il < sizeof(mlnet); il++)
  {
    pos = member_array(mynum, mlnet[il]);

    if (pos < 0)
	continue;

    if (pos > 0)
      localpaths = add_path(localpaths, dir_map[mynum - mlnet[il][pos - 1]]);

    if ((pos >= 0) && (pos < (sizeof(mlnet[il])-1)))
      localpaths = add_path(localpaths, dir_map[mynum - mlnet[il][pos + 1]]);

    if ((pos == 0) || (pos == (sizeof(mlnet[il])-1)))
      if ((mynum / (MAP_SL_MAX + 1)) == 0)
	localpaths = add_path(localpaths, "south");
      else if ((mynum % (MAP_SL_MAX + 1)) == 0)
	localpaths = add_path(localpaths, "west");
      else if ((mynum % (MAP_SL_MAX + 1)) == (MAP_SL_MAX))
	localpaths = add_path(localpaths, "east");
      else
	localpaths = add_path(localpaths, "north");
  }
  return localpaths;
}

/*
 * Dump a ML network, could be paths, roads or rivers
 */
void
dump_dirs_here(int *mlnet)
{
    mixed sqarr, sq;
    int il, il2, il3;
    string fl, ma;

    sqarr = ({});
    for (il = 0; il < sizeof(mlnet); il++)
        sqarr += mlnet[il];

    call_out("dump_dirs2", 1, ({this_player(), sqarr, MAP_SL_MAX }));
}

void
dump_dirs2(mixed *arg)
{
    mixed sqarr;
    int il, il2, il3;
    string fl, ma;
    object tp;

    tp    = (object) arg[0];
    sqarr = arg[1];
    il2   = (int) arg[2];

    for (il3 = il2; ((il3 >= 0) && ((il2 - il3) < 5)); il3--)
    {
	for (fl = "", il = 0; il <= MAP_SL_MAX; il++)
	{
	    if (member_array(MAP_SL(il, il3), sqarr) >= 0)
		fl += " _" + il + "_" + il3 + "_ ";
	    else fl+= " ..... ";
	}
	tell_object(tp, fl + "\n");
    }
    if (il3 >= 0)
	call_out("dump_dirs2", 1, ({tp, sqarr, il3}));
}

public string
print_wizinfo()
{
    string *ihlist;

    ihlist = SECURITY->do_debug("inherit_list", this_object());

    return "Terrain file: " + ihlist[1] + "\nExceptions: " +
	implode(exceptions, "\n") + "\n";
}

void init()
{
    ::init();
    if (this_player()->query_wiz_level())
    {
	add_action("dump_paths_here","mlpaths");
	add_action("dump_roads_here","mlroads");
	add_action("dump_rivers_here","mlrivers");
    }
}


int dump_paths_here()
{
    this_object()->dump_paths();
    return 1;
}

int dump_roads_here()
{
    this_object()->dump_roads();
    return 1;
}

int dump_rivers_here()
{
    this_object()->dump_rivers();
    return 1;
}

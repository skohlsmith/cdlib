/*

  rivers.c

  Water ways in the landscape.

  Each maplocation can be entered with rivers from four directions,
  how the rivers connect within the location and where on the edge
  the river start is similary decided as for paths.

  Also, rivers only go in the four main directions: north, east, south, west
  no diagonal rivers.

     The rivers should not have the capability of looping or exiting the
     ML squares on other points than the given exitpoints.

     To simplify, don't use the corners.

     This version uses the random efun with a second seed argument.

*/
#include <map.h>
#include <macros.h>

static 	mixed	rivermap;	/* The roads in this maplocation */

#define RRND1(x, y, max) random(max, ((y*2) * MAP_WL_MAX) + (x*2))
#define RRND2(x, y, x1, y1, max) random(max, (((y) * (y1) * 2) * MAP_WL_MAX) + ((x) * (x1) * 2))


/*
 * Function name:  query_riverarray
 *
 * Description:    Create for river array for one map location
 *		({
 *		  ({ sl_nn, sl_nn, sl_nn, sl_nn, })
 *		  ({ sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, })
 *		  ({ sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, })
 *		  ({ sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, })
 *		})
 *	Where sl_nn is the indexnumber for an sublocation in the ML.
 *
 * Globals:        rivermap: Stored riverarray for this ML
 *
 */
public mixed
query_riverarray(int x, int y)
{
    int bits, *starts, mid, il;

    if (pointerp(rivermap))
	return rivermap;
    else
	rivermap = this_object()->find_mlriver_zz(x, y);

    if (pointerp(rivermap))
	return rivermap;

    bits = MAP_OBJ->query_rivers(x, y);

    if (bits == 0)
    {
	rivermap = ({});
	return rivermap;
    }

    /*
	Get an SL for each of the bits
     */
    starts = this_object()->choose_river_starts(x, y, bits);

    if (sizeof(starts) < 2)
    {
	rivermap = ({});
	return rivermap;
    }

    /*
     * We must check directions for rivers and build the array so
     * that the river flows from the start SL to the end SL arraywise
     *
     * This is not ready yet...
     */
    if (sizeof(starts) == 2)
	rivermap = ({ this_object()->build_river(starts[0], starts[1]) });
    else
    {
	mid = MAP_SL(1 + RRND1(x, y, 7), 1 + RRND1(x, y, 7));
	rivermap = ({});
	for (il = 0; il < sizeof(starts); il++)
	    rivermap += ({ this_object()->build_river(mid, starts[il]) });
    }
}

public int *
choose_river_starts(int x, int y, int bits)
{
    int *res;

    res = ({});

    if (bits & NORTH)
	res += ({ MAP_SL(8, 1 + RRND2(x, y, x, y + 1,  7)) });

    if (bits & SOUTH)
	res += ({ MAP_SL(0, 1 + RRND2(x, y, x, y - 1,  7)) });

    if (bits & EAST)
	res += ({ MAP_SL(1 + RRND2(x, y, x + 1, y, 7), 8) });

    if (bits & WEST)
	res += ({ MAP_SL(1 + RRND2(x, y, x - 1, y, 7), 0) });

    return res;
}

#define abs(a) ((a)<0 ? -(a) : (a))
#define sgn(a) ((a)<0 ? -1 : ((a) > 0 ? 1 : 0))

public int *
build_river(int sl_from, int sl_to)
{
    int *river;
    int xm, ym, x, y;

    x = MAP_X(sl_from);
    y = MAP_Y(sl_from);
    xm = MAP_X(sl_to);
    ym = MAP_Y(sl_to);
    river = ({});

    while (xm != x || ym != y)
    {
	if (abs(xm - x) > abs(ym - y))
	    x += sgn(xm - x);
	else
	    y += sgn(ym - y);
	river += ({ MAP_SL(x, y) });
    }
    return ({ sl_from }) + river;
}

int
dump_rivers()
{
    int wlx, wly, slx, sly;
    string xyc;

    sscanf(file_name(this_object()), MAP_PATH + "/%s", xyc);

    if (sscanf(xyc, "x%d.%dy%d.%d", wlx, slx, wly, sly) != 4)
	return 0;
    else
    {
	write("River map for MapLocation(x= " + wlx + ", y= " + wly + ")\n");
	this_object()->dump_dirs_here(query_riverarray(wlx, wly));
    }
}


mixed *
find_mlriver_zz(int wlx, int wly)
{
    string fname;
    int arr;
    mixed other;

    fname = MAP_PATH + "/x"+wlx+".0y"+wly+".0";

    if (file_name(this_object()) == fname)
	return 0;

    fname = "x" + wlx + ".0y" + wly + ".0";
    other = map_file(fname);
    if (other)
    {
	rivermap = other->query_riverarray(wlx, wly);
	return rivermap;
    }
    return 0;
}

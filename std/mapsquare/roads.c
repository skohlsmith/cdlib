/*

  roads.c

  Paved roads.

  Each maplocation can be entered with roads from four directions,
  how the roads connect within the location and where on the edge
  the road start is similary decided as for paths.

  Also, roads only go in the four main directions: north, east, south, west
  no diagonal roads.

     The roads should not have the capability of looping or exiting the
     ML squares on other points than the given exitpoints.

     To simplify, don't use the corners.

     This version uses the random efun with a second seed argument.

*/
#include <map.h>
#include <macros.h>

static 	mixed	roadmap;	/* The roads in this maplocation */

#define PRND1(x, y, max) random(max, ((y) * MAP_WL_MAX) + (x))
#define PRND2(x, y, x1, y1, max) random(max, (((y) * (y1)) * MAP_WL_MAX) + ((x) * (x1)))

/*
 * Function name:  query_roadarray
 *
 * Description:    Create for road array for one map location
 *		({
 *		  ({ sl_nn, sl_nn, sl_nn, sl_nn, })
 *		  ({ sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, })
 *		  ({ sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, })
 *		  ({ sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, })
 *		})
 *	Where sl_nn is the indexnumber for an sublocation in the ML.
 *
 * Globals:        roadmap: Stored roadarray for this ML
 *
 */
public mixed
query_roadarray(int x, int y)
{
    int bits, *starts, mid, il;

    if (pointerp(roadmap))
	return roadmap;
    else
	roadmap = this_object()->find_mlroad_zz(x, y);

    if (pointerp(roadmap))
	return roadmap;

    bits = MAP_OBJ->query_roads(x, y);

    if (bits == 0)
    {
	roadmap = ({});
	return roadmap;
    }

    /*
	Get an SL for each of the bits
     */
    starts = this_object()->choose_road_starts(x, y, bits);

    if (sizeof(starts) < 2)
    {
	roadmap = ({});
	return roadmap;
    }

    if (sizeof(starts) == 2)
	roadmap = ({ this_object()->build_road(starts[0], starts[1]) });
    else
    {
	mid = MAP_SL(1 + PRND1(x, y, 7), 1 + PRND1(x, y, 7));
	roadmap = ({});
	for (il = 0; il < sizeof(starts); il++)
	    roadmap += ({ this_object()->build_road(mid, starts[il]) });
    }
}

public int *
choose_road_starts(int x, int y, int bits)
{
    int *res;

    res = ({});

    if (bits & NORTH)
	res += ({ MAP_SL(8, 1 + PRND2(x, y, x, y + 1,  7)) });

    if (bits & SOUTH)
	res += ({ MAP_SL(0, 1 + PRND2(x, y, x, y - 1,  7)) });

    if (bits & EAST)
	res += ({ MAP_SL(1 + PRND2(x, y, x + 1, y, 7), 8) });

    if (bits & WEST)
	res += ({ MAP_SL(1 + PRND2(x, y, x - 1, y, 7), 0) });

    return res;
}

#define abs(a) ((a)<0 ? -(a) : (a))
#define sgn(a) ((a)<0 ? -1 : ((a) > 0 ? 1 : 0))

public int *
build_road(int sl_from, int sl_to)
{
    int *road;
    int xm, ym, x, y;

    x = MAP_X(sl_from);
    y = MAP_Y(sl_from);
    xm = MAP_X(sl_to);
    ym = MAP_Y(sl_to);
    road = ({});

    while (xm != x || ym != y)
    {
	if (abs(xm - x) > abs(ym - y))
	    x += sgn(xm - x);
	else
	    y += sgn(ym - y);
	road += ({ MAP_SL(x, y) });
    }
    return ({ sl_from }) + road;
}

int
dump_roads()
{
    int wlx, wly, slx, sly;
    string xyc;

    sscanf(file_name(this_object()), MAP_PATH + "/%s", xyc);

    if (sscanf(xyc, "x%d.%dy%d.%d", wlx, slx, wly, sly) != 4)
	return 0;
    else
    {
	write("Road map for MapLocation(x= " + wlx + ", y= " + wly + ")\n");
	this_object()->dump_dirs_here(query_roadarray(wlx, wly));
    }
}


mixed *
find_mlroad_zz(int wlx, int wly)
{
    string fname;
    int arr;
    mixed other;

    fname = MAP_PATH + "/x" + wlx + ".0y" + wly + ".0";

    if (file_name(this_object()) == fname)
	return 0;

    fname = "x" + wlx + ".0y" + wly + ".0";
    other = map_file(fname);
    if (other)
    {
	roadmap = other->query_roadarray(wlx, wly);
	return roadmap;
    }
    return 0;
}

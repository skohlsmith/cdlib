/*************************************************************


  Randomized paths for an NxN square. (Ver 2.0)

  Constraints:

     The enter/exit points must depend on the edge id, i.e
         WL:WL

     Choose (0-2) connectpoints / edge.

     The pathpoints must depend on WL.SL

     The paths should not have the capability of looping or exiting the
     WL squares on other points than the given exitpoints.

     To simplify, don't use the corners.

     This version uses the random efun with a second seed argument, not yet...

*/

#define	PRIM		701	/* 3 large, different primes */
#define	PRIM2		14009	/* There's a file of them here somewhere :-) */
#define	PRIM3		54001


#define SEED_X1WL(wlx, wly) ((wly)*MAP_WL_MAX + (wlx)) % PRIM;
#define SEED_X2WL(wlx1, wly1, wlx2, wly2) (((wly1)*MAP_WL_MAX + (wlx1)) * ((wly2)*MAP_WL_MAX + (wlx2))) % PRIM;
#define SEED_1WL(wlx, wly) SEED_X1WL((wlx)+PRIM2, (wly)+PRIM3)
#define SEED_2WL(wlx1, wly1, wlx2, wly2) SEED_X2WL((wlx1)+PRIM2, (wly1)+PRIM3,\
(wlx2)+PRIM2, (wly2)+PRIM3)

/*
 * Function name: query_roadpath
 * Description:   Gives the roadpaths in a given WL.SL square.
 * Arguments:     wlx: World coordinate X.
 *                wly: World coordinate Y.
 *                slx: Square coordinate X.
 *                sly: Square coordinate Y.
 * Returns:       Paths as an array from the center of the square:
 *                ({ "nw", "n", "ne", "e", "se", "s", "sw", "w" })
 *                Any or none can be included.
 */
public string *
query_roadpath(int wlx, int wly, int slx, int sly)
{
  mixed *pathswl;

  /* Get all paths in this WL
  */
  pathswl = this_object()->query_pathswl(wlx, wly);

  return this_object()->query_dirs_here(wlx, wly, slx, sly, pathswl);
}


/* Forward declarations */

static int same_edge(int from, int to);
static int *query_rnd_connections(int wlx1, int wly1, int wlx2, int wly2);
int query_rnd_seed(int wlx, int wly, int max);
static int * query_rnd_path(int wlx, int wly, int from, int to);
mixed * find_wlzz(int wlx, int wly);

/*
 * Function name:  query_pathswl
 *
 * Description:    Create for a WL a path array:
 *		({
 *		  ({ sl_nn, sl_nn, sl_nn, sl_nn, })
 *		  ({ sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, })
 *		  ({ sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, })
 *		  ({ sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, sl_nn, })
 *		})
 *	Where sl_nn is the indexnumber for an SL in the WL, i.e.
 *	y_coord*SLM + x_coord (0-225)
 *
 *	Diffvalues:  my_pos - to_pos
 *		nw: SLM, n: SLM+1, ne: SLM+2, e: 1,
 *		se: -(SLM), s: -(SLM+1), sw: -(SLM+2), w: -1
 *
 * Globals:        wl_pathmap: Stored patharray for this WL
 *
 */

mixed *wl_pathmap;

mixed *
query_pathswl(int wlx, int wly)
{
  mixed *con_sl;
  int pos, from, to, fsl, tsl;

  if (wl_pathmap)
      return wl_pathmap;

  if (find_wlzz(wlx, wly))
      return wl_pathmap;

  wl_pathmap = ({});

  con_sl = query_rnd_connections(wlx, wly, wlx - 1, wly) +
    query_rnd_connections(wlx, wly, wlx + 1, wly) +
    query_rnd_connections(wlx, wly, wlx, wly - 1) +
    query_rnd_connections(wlx, wly, wlx, wly + 1);

  if (sizeof(con_sl) < 1)
      return wl_pathmap;

  while (sizeof(con_sl) > 1)
  {
    from = query_rnd_seed(wlx, wly * sizeof(con_sl), sizeof(con_sl));
    fsl = con_sl[from];
    con_sl = exclude_array(con_sl, from, from);
    to = query_rnd_seed(wlx, wly * sizeof(con_sl), sizeof(con_sl));
    tsl = con_sl[to];
    con_sl = exclude_array(con_sl, to, to);
    if (same_edge(fsl, tsl) && (sizeof(con_sl) > 1))
    {
	to = query_rnd_seed(wlx, wly * sizeof(con_sl), sizeof(con_sl));
	pos = con_sl[to];
	con_sl = ({ tsl }) + exclude_array(con_sl, to, to);
	tsl = pos;
    }
    wl_pathmap +=  ({ ({fsl}) + query_rnd_path(wlx, wly, fsl, tsl) + ({tsl}) });
  }
  if (sizeof(con_sl) == 1)
    wl_pathmap += ({ ({ con_sl[0] }) });

  return wl_pathmap;
}

static int
same_edge(int from, int to)
{
    if (((from % (MAP_SL_MAX+1)) == 0) &&
	((to % (MAP_SL_MAX+1)) == 0))
	return 1;
    else if (((from % (MAP_SL_MAX+1)) == MAP_SL_MAX) &&
	((to % (MAP_SL_MAX+1)) == MAP_SL_MAX))
	return 1;
    else if (((from / (MAP_SL_MAX+1)) == 0) &&
	((to / (MAP_SL_MAX+1)) == 0))
	return 1;
    else if (((from / (MAP_SL_MAX+1)) == MAP_SL_MAX) &&
	((to / (MAP_SL_MAX+1)) == MAP_SL_MAX))
	return 1;
    return 0;
}


static int *
query_rnd_connections(int wlx1, int wly1, int wlx2, int wly2)
{
  int seed, num_con, il, pair;
  int *choose_from;

  seed = SEED_2WL(wlx1, wly1, wlx2, wly2); if (seed<0) seed = -seed;

  num_con = (seed / 16) % 3;
  choose_from = ({});

  if ((num_con > 2) || (!num_con))
    return ({});

  if ((wlx1 == wlx2) && (wly1<wly2))  /* south */
    for (il=MAP_SL(1, MAP_SL_MAX);il<=MAP_SL(MAP_SL_MAX-1, MAP_SL_MAX);il++)
      choose_from += ({ il });

  else if ((wlx1 == wlx2) && (wly1>wly2)) /* north */
    for (il=MAP_SL(1, 0);il<=MAP_SL(MAP_SL_MAX-1, 0);il++)
      choose_from += ({ il });

  else if ((wlx1 < wlx2) && (wly1 == wly2)) /* east */
    for (il=MAP_SL(MAP_SL_MAX, 1);il<=MAP_SL(MAP_SL_MAX, MAP_SL_MAX-1);il+=MAP_SL_MAX+1)
      choose_from += ({ il });

  else if ((wlx1 > wlx2) && (wly1 == wly2)) /* west */
    for (il=MAP_SL(0, 1);il<=MAP_SL(0, MAP_SL_MAX-1);il+=MAP_SL_MAX+1)
      choose_from += ({ il });

  if (num_con == 1)
    return ({ choose_from[seed % (MAP_SL_MAX-1)] });
  else
    return ({ choose_from[seed % (MAP_SL_MAX-1)],
	      choose_from[(seed / (MAP_SL_MAX)) % (MAP_SL_MAX-1)] });
}


/* static */

int
query_rnd_seed(int wlx, int wly, int max)
{
  int seed;

  seed = SEED_1WL(wlx, wly); if (seed<0) seed = - seed;

  return seed % max;
}


/*
 * This just builds a straight path between the points right now
 *
 */
static int *
query_rnd_path(int wlx, int wly, int from, int to)
{
  int x, y;

  if (from == to) return ({});
  if (member_array(from-to, MAP_SL_DIFFS)>=0) { /* Opposite edges */
      if (((from % (MAP_SL_MAX+1)) != MAP_SL_MAX) &&
	  ((from % (MAP_SL_MAX+1)) != 0))
      return ({});
      if (((to % (MAP_SL_MAX+1)) != MAP_SL_MAX) &&
	  ((to % (MAP_SL_MAX+1)) != 0))
      return ({});
      if ((to % (MAP_SL_MAX+1)) ==  (from % (MAP_SL_MAX+1)))
      return ({});
  }

  x = ((from % (MAP_SL_MAX+1)) + (to % (MAP_SL_MAX+1))) / 2;
  y = ((from / (MAP_SL_MAX+1)) + (to / (MAP_SL_MAX+1))) / 2;

  x = MAP_SL(x, y); /* Middle point */

  if ((x != from) && (x != to)) {
      return
	  query_rnd_path(wlx, wly, from, x) + ({ x }) +
	      query_rnd_path(wlx, wly, x, to);
  }
  else
      return ({});
}

int
dump_paths()
{
    int wlx, wly, slx, sly;
    string xyc;

    sscanf(file_name(this_object()), MAP_PATH + "/%s", xyc);

    if (sscanf(xyc, "x%d.%dy%d.%d", wlx, slx, wly, sly) != 4)
	return 0;
    else
    {
	write("Path map for MapLocation(x= " + wlx + ", y= " + wly + ")\n");
	this_object()->dump_dirs_here(query_pathswl(wlx, wly));
    }
}


mixed *
find_wlzz(int wlx, int wly)
{
    string fname;
    int arr;
    mixed other;

    fname = MAP_PATH + "/x"+wlx+".0y"+wly+".0";

    if (file_name(this_object()) == fname)
	return 0;

    fname = "x"+wlx+".0y"+wly+".0";
    other = map_file(fname);
    if (other) {
	wl_pathmap = other->query_pathswl(wlx, wly);
	return wl_pathmap;
    }
    return 0;
}

inherit "/std/object.c";

#include "/config/sys/map_defines.h"
#include "/secure/std.h"

int out_of_bounds(int x, int y, int xw, int yw);
void do_draw(int x, int y, int x_width, int y_width);
varargs mixed get_area_from_corner(int x, int y, int x_width, int y_width);
string fix_char(string str);
string read_col(int x, int y, int num);

/*
 * Draw one map character on the players terminal.
 */
int get_one_square(int x, int y) {

    if(out_of_bounds(x, y, 1, 1)) {
	write("Someone spilled spam on that part of the map.\n");
	return 1;
    }

    do_draw(x, y, 1, 1);
    return 1;
}

/*
 * Draw a map with x,y as centre and rad as radius using
 * standard map characters.
 */
draw_area_around(int x, int y, int rad) {

    if(out_of_bounds(x - rad, y + rad, rad * 2 + 1, rad * 2 + 1)) {
	write("Someone spilled spam on that part of the map.\n");
	return 1;
    }

    do_draw(x - rad, y + rad, rad * 2 + 1, rad * 2 + 1);
    return 0;
}

/*
 * Draw a map with the top left corner in x,y. Use the standard
 * map characters.
 */
varargs int draw_area_from_corner(int x, int y, int range, int y__width) {

    if(range < 1 || range > 40 || y__width > 40) {
	write("Illegal range\n");
	return 1;
    }

    if(!y__width)
	y__width = range;

    if(out_of_bounds(x, y, range, y__width)) {
	write("Someone spilled spam on that part of the map.\n");
	return 1;
    }

    do_draw(x, y, range, y__width);

    return 0;
}

/*
 * Read an area from the map, convert it to standard map
 * characters and print them.
 */
void do_draw(int x, int y, int x__width, int y__width) {

    int i, j;
    mixed mape;

    mape = get_area_from_corner(x, y, x__width, y__width);

    for(i = 0 ; i < y__width ; i++) {
	for(j = 0 ; j < x__width ; j++)
	    write(fix_char(extract((string)(mape[j]), i*2, i*2)) + " ");
	write("\n");
    }
}

/*
 * Read an area into an array of arrays and return the array.
 */
varargs mixed
get_area_from_corner(int x, int y, int x__width, int y__width) {

    mixed map;
    int i;

    map = ({ });

    for(i = 0 ; i < x__width ; i++, x++)
	map += ({ read_col(x, y, y__width?y__width:x__width) });
    return map;
}

/*
 * Read one column from tha map database and return the string
 * containing that column.
 */
string
read_col(int x, int y, int num) {

    if(x >= 0) {
/*      write("x = " + x + "\n");
	write("y = " + y + "\n");
	write("Y_TOT = " + Y_TOT + "\n");
	write("MAXY = " + MAXY + "\n");
	write("UGLY_FIX = " + UGLY_FIX + "\n");
	write("MAP_DEPTH = " + MAP_DEPTH + "\n");
	write(MAP_DEPTH*((x+1)*Y_TOT-(y+MAXY+UGLY_FIX)) + "\n");
 */
	return read_bytes(MAP_FILE, MAP_DEPTH*((x+1)*Y_TOT-(y+MAXY+UGLY_FIX)), num*2);
    }
    x = -x;
/*    write(-1*(x*Y_TOT-(MAXY-y+1)) + "\n"); */
    return read_bytes(MAP_FILE, MAP_DEPTH*(-1*(x*Y_TOT-(MAXY-y))), num*2);
}

/*
 * Write a number of bytes in one column into the map database.
 */
int write_raw_square(int x, int y, string str) {
/*    write(x + ", " + y + ", " + strlen(str) + "\n"); */
    if(strlen(str) != 1 && strlen(str) != 2)
	return 0;
    if(out_of_bounds(x, y, 0, 0))
	return 0;
    if(x >= 0) {
/*	write(MAP_DEPTH*((x+1)*Y_TOT-(y+MAXY+UGLY_FIX)) + ", " + str[1] + "\n");
*/
	return write_bytes(MAP_FILE, MAP_DEPTH*((x+1)*Y_TOT-(y+MAXY+UGLY_FIX)), str);
    }
    x = -x;
    return write_bytes(MAP_FILE, MAP_DEPTH*(-1*(x*Y_TOT-(MAXY-y))), str);
}

/*
 * Check to see if we are accessing locations outside the map.
 */
int out_of_bounds(int x, int y, int xw, int yw) {

    if(x < MINX)
	return 1;

    if(y > MAXY)
	return 1;

    if(x + xw > MAXX)
	return 1;

    if(y + yw < MINY)
	return 1;

    return 0;
}

/*
 * Take raw map characters and remove RE and domain information.
 * Then return the corresponding printable character from map_defines.h.
 */
string fix_char(string str) {

    return MAP_CHARS[str[0] & PLAIN];
}

/*
 * Read one ML from the database and return the information
 * without removing RE and domain information.
 */
int get_one_raw_square(int x, int y) {

    if(out_of_bounds(x, y, 1, 1))
	return 0;

    return read_col(x, y, 1)[0];
}

/*
 * Set or remove the domain bit in one ML. If on==0 we clear
 * the bit, otherwise we set it.
 */
int set_domain_bit(int x, int y, int on) {

    int i;

    if(!(i = get_one_raw_square(x, y)))
	return 0;

    if(on && !(i & DOMAIN_BIT))
	return write_raw_square(x, y, strchar(i & DOMAIN_BIT));
    else if(!on && (i & DOMAIN_BIT))
	return write_raw_square(x, y, strchar(i ^ DOMAIN_BIT));
    return -1;
}

/*
 * Set or remove the random encounter (RE) bit in one ML. If on==0 we clear
 * the bit, otherwise we set it.
 */
int set_encounter_bit(int x, int y, int on) {

    int i;

    if(!(i = get_one_raw_square(x, y)))
	return 0;

    if(on && !(i & RANDOM_ENCOUNTER))
	return write_raw_square(x, y, strchar(i & RANDOM_ENCOUNTER));
    else if(!on && (i & RANDOM_ENCOUNTER))
	return write_raw_square(x, y, strchar(i ^ RANDOM_ENCOUNTER));
    return -1;
}

/*
 * Change the terrain type in one ML. Keep RE and domain information.
 */
int modify_square(int x, int y, int newtype) {

    int i;
    int e_map;
    int d_map;

    if (WIZ_CHECK < WIZ_LORD)
	return 0;
    if (this_interactive() != this_player())
	return 0;

    if(!(i = get_one_raw_square(x, y)))
	return 0;

    e_map = i & RANDOM_ENCOUNTER;
    d_map = i & DOMAIN_BIT;

    return write_raw_square(x, y, strchar(newtype | e_map | d_map));
}

/*
 * Take a name of a terrain type and return the number of that terrain type.
 */
int tstring_to_tint(string tstring) {

    return member_array(tstring, TERRAIN_NAMES);
}

/*
 * Return one array for every level of the map. Each array contains
 * raw information about one ML and its eight neighbours.
 */
mixed get_nine(int x, int y) {

    mixed arr;
    string tmp;
    int i;
    int j;
    int k;

    if(out_of_bounds(x-1, y+1, 2, 2))
	return ({});
    arr = allocate(MAP_DEPTH);
    for(k = 0 ; k < MAP_DEPTH ; k++) {
	arr[k] = ({});
    }
    for(i = 0 ; i < 3 ; i++) {
	tmp = read_col(x+i-1, y+1, 3);
	for(j = 0 ; j < 3 ; j++) {
	    for(k = 0 ; k < MAP_DEPTH ; k++) {
		arr[k] += ({tmp[j * MAP_DEPTH + k]});
	    }
	}
    }
    return arr;
}

/*
 * Set river information in level 1 of the map. The string
 * should contain the directions the road emerges from the
 * specified ML.
 */

int set_river(int x, int y, string dirs, string flows) {

    int len;
    int i;
    int value;
    string *comp;
    string *flow;
    string *dir;
    string *tmp;
    string square;
    string t;
    int ndir;

    if(!IS_RIVER(x, y)) {
	return -1;
    }

    if((len = strlen(dirs)) > 4) {
	return 0;
    }

    if(!dirs || strlen(dirs) < 1) {
	return 0;
    }

    if(out_of_bounds(x, y, 1, 1)) {
	return -2;
    }

    square = read_col(x, y, 2);
    t = extract(square, 0, 0);

    if(strlen(dirs) == 1)
	dirs += "R";

    dir = explode(dirs, "");
    len = sizeof(dir);
    ndir = 0;

    for(i = 0 ; i < len ; i++) {
	switch(dir[i]) {
	case "n":
	    value |= NORTH;
	    ndir++;
	    break;
	case "e":
	    value |= EAST;
	    ndir++;
	    break;
	case "s":
	    value |= SOUTH;
	    ndir++;
	    break;
	case "w":
	    value |= WEST;
	    ndir++;
	    break;
	case "0":
	    value = 0;
	    ndir++;
	    break;
	case "R":
	    break;
	default:
	    return -3;
	    break;
	}
    }

    if(flows == "0")
	flows = "";
    len = strlen(flows);

    if(value) {
	if(len == 0) {
	    if(ndir == 4) {
		/*
		 * Rivers are going in all directions and we cant
		 * handle it without one or three exits.
		 */
		return -4;
	    } else {
		/*
		 * All rivers are flowing in, either to a sea, lake
		 * or some strange hole in the ground.
		 */
		if(! (value & NORTH))
		    value |= OUT_NORTH;
		else if(! (value & EAST))
		    value |= OUT_EAST;
		else if(! (value & SOUTH))
		    value |= OUT_SOUTH;
		else if(! (value & WEST))
		    value |= OUT_WEST;
		write_raw_square(x, y, t + strchar(value));
		return 1;
	    }
	}

	if(len > ndir)
	    /*
	     * More exits than there are rivers? No way!
	     */
	    return -5;

	if(len == ndir) {
	    if(len == 4)
		/*
		 * We can't handle a source when having 4 rivers.
		 */
		return -4;
	    /*
	     * All rivers are flowing out. Then we have a source
	     * here and set a dummy exit in an unused direction.
	     */
	    if(! (value & NORTH))
		value |= OUT_NORTH;
	    else if(! (value & EAST))
		value |= OUT_EAST;
	    else if(! (value & SOUTH))
		value |= OUT_SOUTH;
	    else if(! (value & WEST))
		value |= OUT_WEST;
	    value |= INVERTED_EXITS;
	    write_raw_square(x, y, t + strchar(value));
	    return 1;
	}
	if(len == 1) {
	    /*
	     * One single exit. Set it and set invert bit.
	     */
	    switch(flows) {
	    case "n":
		if(!(value & NORTH))
		    return -6;
		value |= OUT_NORTH;
		break;
	    case "e":
		if(!(value & EAST))
		    return -6;
		value |= OUT_EAST;
		break;
	    case "s":
		if(!(value & SOUTH))
		    return -6;
		value |= OUT_SOUTH;
		break;
	    case "w":
		if(!(value & WEST))
		    return -6;
		value |= OUT_WEST;
		break;
	    default:
		return -7;
		break;
	    }
	    write_raw_square(x, y, t + strchar(value));
	    return 1;
	}
	if(len == 2 || len == 3) {
	    /*
	     * Now we have to find the missing direction
	     * and check to see that it is the only missing.
	     * That direction is where the river enters.
	     */
	    flow = explode(flows, "");
	    flow = dir - flow;
	    if(sizeof(flow) != 1)
		return -8;
	    switch(flow[0]) {
	    case "n":
		value |= OUT_NORTH;
		break;
	    case "e":
		value |= OUT_EAST;
		break;
	    case "s":
		value |= OUT_SOUTH;
		break;
	    case "w":
		value |= OUT_WEST;
		break;
	    default:
		return -7;
		break;
	    }
	    write_raw_square(x, y, t + strchar(value));
	    return 1;
	}
     }
    write_raw_square(x, y, t + strchar(value|INVERTED_EXITS));
    return 1;
}

/*
 * Set road information in level 1 of the map. The string
 * should contain the directions the road emerges from the
 * specified ML.
 */

int set_road(int x, int y, string dirs) {

    int len;
    int i;
    int value;
    string *comp;
    string square;
    string t;
    int r;
    int ok;

    if(!IS_ROAD(x, y)) {
	return -1;
    }

    if((len = strlen(dirs)) > 4)
	return 0;

    if(!dirs || strlen(dirs) < 2)
	return 0;

    comp = explode(dirs, "");

    if(out_of_bounds(x, y, 1, 1))
	return 0;

    square = read_col(x, y, 2);
    t = extract(square, 0, 0);
    r = square[1] & 0x61;

    for(i = 0 ; i < len ; i++) {
	switch(comp[i]) {
	case "n":
	    value |= NORTH;
	    break;
	case "e":
	    value |= EAST;
	    break;
	case "s":
	    value |= SOUTH;
	    break;
	case "w":
	    value |= WEST;
	    break;
	case "0":
	    value = 0;
	    break;
	case "R":
	    ok = 1;
	    break;
	default:
	    return 0;
	    break;
	}
    }
    if(!ok)
	return 0;
    r |= value;
    write_raw_square(x, y, t + strchar(r));

    return 1;
}

int query_river(int x, int y) {

    int *arr;
    int ret;
    int tmp;
    int exit;
    int in;
    int out;

    if(out_of_bounds(x, y, 0, 0))
       return 0;

    if(IS_RIVER(x, y)) {
	tmp =  get_nine(x, y)[1][4];
	if(ret = (tmp & ALL_DIRS))
	    return 0;
	switch(tmp & OUT_WEST) {
	case OUT_NORTH:
	    exit = RIVER_OUT_NORTH;
	    break;
	case OUT_EAST:
	    exit= RIVER_OUT_EAST;
	    break;
	case OUT_SOUTH:
	    exit = RIVER_OUT_SOUTH;
	    break;
	case OUT_WEST:
	    exit = RIVER_OUT_WEST;
	    break;
	default:
	    return 0;
	    break;
	}
	if(tmp & INVERTED_EXITS) {
	    ret |= exit;
	} else {
	    if((tmp & NORTH) && exit != RIVER_OUT_NORTH)
		ret |= RIVER_OUT_NORTH;
	    if((tmp & EAST) && exit != RIVER_OUT_EAST)
		ret |= RIVER_OUT_EAST;
	    if((tmp & SOUTH) && exit != RIVER_OUT_SOUTH)
		ret |= RIVER_OUT_SOUTH;
	    if((tmp & WEST) && exit != RIVER_OUT_WEST)
		ret |= RIVER_OUT_WEST;
	}
	if((tmp & NORTH && (tmp & OUT_WEST) != OUT_NORTH) ||
	(tmp & EAST && (tmp & OUT_WEST) != OUT_EAST) ||
	(tmp & SOUTH && (tmp & OUT_WEST) != OUT_SOUTH) ||
	(tmp & WEST && (tmp & OUT_WEST) != OUT_WEST)) {
	    if(tmp & INVERTED_EXITS)
		ret |= RIVER_SOURCE;
	    else
		ret |= RIVER_END;
	}
	return ret;
    }

    arr = get_nine(x, y)[1];
    ret = 0;

    ret = (arr[3] & SOUTH) >> 2;
    ret |= (arr[7] & WEST) >> 2;
    ret |= (arr[5] & NORTH) << 2;
    ret |= (arr[1] & EAST) << 2;

    if(ret & NORTH) {
	write("arr[3] & OUT_WEST = " + (arr[3] & OUT_WEST) + ", OUT_SOUTH = " + OUT_SOUTH + ", arr[3] & INVERTED_EXITS = " + (arr[3] & INVERTED_EXITS) + "\n");
	if((arr[3] & OUT_WEST) == OUT_SOUTH && (arr[3] & INVERTED_EXITS)) {
	    in = 1;
	} else {
	    ret |= RIVER_OUT_NORTH;
	    out = 1;
	}
    }

    if(ret & EAST) {
	if((arr[7] & OUT_WEST) == OUT_WEST && (arr[7] & INVERTED_EXITS)) {
	    in = 1;
	} else {
	    write("Setting RIVER_OUT_EAST\n");
	    ret |= RIVER_OUT_EAST;
	    out = 1;
	}
    }
    if(ret & SOUTH) {
	write("We have a river going south\n");
	if((arr[5] & OUT_WEST) == OUT_NORTH && (arr[5] & INVERTED_EXITS)) {
	    in = 1;
	} else {
	    ret |= RIVER_OUT_SOUTH;
	    out = 1;
	}
    }
    if(ret & WEST) {
	if((arr[1] & OUT_WEST) == OUT_EAST && (arr[1] & INVERTED_EXITS)) {
	    in = 1;
	} else {
	    ret |= RIVER_OUT_WEST;
	    out = 1;
	}
    }
    if(!in)
	ret |= RIVER_SOURCE;
    if(!out)
	ret |= RIVER_END;
    return ret;
}

int query_road(int x, int y) {

    int *arr;
    int ret;

    if(out_of_bounds(x, y, 0, 0))
       return 0;

    if(IS_ROAD(x, y))
	return get_nine(x, y)[1][4] & ALL_DIRS;

    arr = get_nine(x, y)[1];

    ret = (arr[3] & SOUTH) >> 2;
    ret |= (arr[7] & WEST) >> 2;
    ret |= (arr[5] & NORTH) << 2;
    ret |= (arr[1] & EAST) << 2;

    return ret;
}


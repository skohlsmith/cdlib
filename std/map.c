#pragma save_binary

inherit "/std/map/map";

#include <macros.h>
#include <stdproperties.h>

#define AROUND 0
#define FROM_CORNER 1

int type;
int center;
int x_corner, y_corner;
int x_width;
int y_width;
int radius;

void set_type(int t);
void set_center(int x, int y);
void set_radius(int r);
void set_x_width(int x);
void set_y_width(int y);

public void
init()
{
    add_action("read_map", "read");
}

/*
 * Function name: reset
 * Description:   Reset the map
 */
void
create_object()
{
    add_prop(OBJ_I_WEIGHT, 20);
    add_prop(OBJ_I_VOLUME, 20);
    add_prop(OBJ_I_VALUE, 5);
    set_name("map");
    set_type(AROUND);
    set_center(7, 7);
    set_radius(6);
    /*
     * This is an example of how you give the top left corner of the map
     * and the width and height of the map, instead of the center and
     * a radius.
     *
     * set_type(FROM_CORNER);
     * set_corner(-100, -450);
     * set_x_width(14);
     * set_y_width(6);
     */
    set_short("map of the area around Larstown");
    set_pshort("maps of the area around Larstown");
    set_long("This is a map made by a skilled hand on a piece of vellum. It\n" +
	     "looks very old. Maybe it is a treasure map?\n");
}

/*
 * Function name: read_scroll
 * Description:   Read the text
 */
static int
read_map(string str)
{
    if(!str || !id(str))
	return 0;
    if(type && x_width > 0 && y_width > 0)
	if(draw_area_from_corner(x_corner, y_corner, x_width, y_width)) {
	    write("The map falls to pieces.\n");
	    destruct();
	}
	else
	    return 1;
    if(!type && radius > 0)
	if(draw_area_around(x_corner, y_corner, radius)) {
	    destruct();
	    write("The map falls to pieces.\n");
	}
	else
	    return 1;
    destruct();
    return 1;
}

void
set_type(int t) {
    type = t;
}

void
set_center(int x, int y) {
    x_corner = x;
    y_corner = y;
}

void
set_corner(int x, int y) {
    x_corner = x;
    y_corner = y;
}

void
set_radius(int r) {
    radius = r;
}

void
set_x_width(int x) {
    x_width = x;
}

void
set_y_width(int y) {
    y_width = y;
}

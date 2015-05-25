/*
 * This file contains information about the size and contents of
 * the map of your world. It is used by /std/map/map.c.
 */

#define MAP_DEF

#define MAXY 4          /* Maximum value Y may have */
#define MINY -5         /* Minimum value Y may have */
#define MAXX 9          /* Maximum value X may have */
#define MINX -5         /* Minimum value X may have */
#define UGLY_FIX 2      /* This depends on MAXY and MINY.
			 * If MINY < 0 and MAXY - abs(MINY) == 1
			 * then UGLY_FIX should be 2
			 */
#define MAP_DEPTH 1     /* Number of bytes for each map location */
#define Y_TOT 10        /* How many are there between MAXY and MINY, 0 included */

#define MAP_SL_MAX 8            /* 0 - 8 */
#define MAP_WL_MAX 9999
#define MAP_SL_SIZE             5
#define MAP_SL(x, y) ((y) * (MAP_SL_MAX + 1) + (x))
#define MAP_X(sl) ((sl) % (MAP_SL_MAX + 1))
#define MAP_Y(sl) ((sl) / (MAP_SL_MAX + 1))

#define MAP_SL_DIFFS ({ (MAP_SL_MAX + 2), (MAP_SL_MAX + 1),    \
		        (MAP_SL_MAX), -1, -(MAP_SL_MAX + 2),   \
		        -(MAP_SL_MAX + 1), -(MAP_SL_MAX), 1 })

#define MAP_SL_DIRS   ({ "southwest", "south",   \
			 "southheast", "east", "northeast",   \
			 "north", "northwest", "west" })

/*
 * Macros to check if a ML should contain road or river information.
 */
#define IS_RIVER(x,y) (((x) + (y)) % 2 == 0)
#define IS_ROAD(x,y)(((x) + (y)) % 2)

/*
 * Each direction needs a bit in the database
 */
#define NORTH 0x2
#define EAST  0x4
#define SOUTH 0x8
#define WEST  0x10
#define ALL_DIRS 0x1E

/*
  Name of the file holding the map information
*/
#define MAP_FILE "/d/Standard/a_map"

/*
  Info on bit patterns in mapfile
*/
#define PLAIN             0x3F
#define RANDOM_ENCOUNTER  0x40
#define DOMAIN_BIT        0x80

/*
  Path to where all maprooms will be created
*/
#define MAP_PATH "/d/Standard/map"

/*
 * MAP_CHARS
 *
 * Printable char for each mapsymbol.
 *
 */
#define MAP_DEFAULT_CHAR "."
#define MAP_CHARS     ({ ".",       /* default       */\
			 ".",       /* space        */\
			 "O",       /* planet        */\
			 "#",       /* asteroid belt */\
			 "*",       /* star base     */\
			 " ",       /* black hole    */\
			 })

/*
 * MAP_ROOMS
 *
 * Mapsymbols and corresponding filenames of terrain rooms 
 *
 */
#define MAP_DEFAULT_ROOM "/d/Standard/terrain/space"
#define MAP_ROOMS ({ \
                     "/d/Standard/terrain/space",         \
		     "/d/Standard/terrain/space",         \
		     "/d/Standard/terrain/planet",        \
		     "/d/Standard/terrain/asteroid_belt", \
		     "/d/Standard/terrain/star_base",     \
		     "/d/Standard/terrain/black_hole",    \
		     })

#define TERRAIN_NAMES ({ \
 		        "space",         \
 		        "space",         \
 		        "planet",        \
 		        "asteroid_belt", \
 		        "star_base",     \
 		        "black_hole",    \
			})
				     

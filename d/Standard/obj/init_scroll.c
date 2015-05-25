/*
 * Copyright (c) 1991 Chalmers Computer Society
 *
 * This code may not be copied or used without the written permission
 * from Chalmers Computer Society.
 */
#pragma strict_types

inherit "/std/scroll";

#include <stdproperties.h>
#include "/config/sys/local.h"

void
create_scroll()
{
    seteuid(getuid(this_object()));
    add_prop(OBJ_I_NO_DROP, "Don't drop the slate, it holds valueable " +
			    "information!\n");
    set_name("slate");
    set_short("slate displaying 'READ ME!'");
    set_long("The slate has a large, square display with backlighting. " +
	     "On the screen the text 'READ ME!' is displayed in big letters.\n");
    set_autoload();
    set_file(APPRENTICE_SCROLL_FILE);
}

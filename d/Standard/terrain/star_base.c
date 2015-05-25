inherit "/std/mapsquare";

void create_mapsquare(int wlx, int wly, int slx,int sly)
{
    ::create_mapsquare(wlx, wly, slx, sly);
	
	set_short("Star base");
	if (sizeof(paths) > 0)
	    pdesc = "\n" + break_string("There are paths leading " + 
				        get_pdesc(paths) + ".", 76);
	else
	    pdesc = "";


	if(slx == 4 && sly == 4) {                     /* Center of base */
	    set_long("This is the centre of the starbase." +
		  "They just started building the power plant here.\n" +
		  "You can fly in any direction.\n");
    } else if((slx > 2 && slx < 6) && (sly > 2 && sly < 6)) {  
	set_long("This is near the midle of the base. " +
		  "There are lots od framework in place here.\n" +
		  "You can walk in any direction.\n");
    } else if((slx > 1 && slx < 7) && (sly > 1 && sly < 7)) {  
	set_long("This is the level of the base that will contain quarters. " +
		  pdesc + "\n" +
		  "You can fly in any direction.\n");
    } else if((slx > 0 && slx < 8) && (sly > 0 && sly < 8)) { 
	set_long("You are just inside the hull of the star base.\n" +
		 "This is where the cargo holds will be.\n" +
		  pdesc + "\n" +
		  "You can fly in fly direction.\n");
    } else {                  
	set_long("You are just outside what looks like a half finished\n " +
		 "star base bigger than Darth Vader's Death Star.\n" +
                 "Now the place seems to be deserted. " +
		  pdesc + "\n" +
		      "You can fly in any direction.\n");
    }
	set_noshow_obvious(1);
   
    }


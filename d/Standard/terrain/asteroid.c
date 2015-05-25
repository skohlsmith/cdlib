inherit "/std/mapsquare";

void create_mapsquare(int wlx, int wly, int slx,int sly)
{
    ::create_mapsquare(wlx, wly, slx, sly);
	
	set_short("Asteroid belt");

	if(query_random_encounter())
	    if(random(9, wlx + 5) == slx && random(9, wly + 1) == sly) {
		set_long("You are maneuvering through an asteroid belt.\n" +
			 "Suddenly you make a mistake and hit a huge rock.\n" +
			 "Luckily your ship survives the crash.\n" +
			 "You can go in any direction.\n");
/*		move_random_encounter(); */
	    }
	    else
		set_long("With great skill you take your ship through.\n" +
		 "an asteroid belt.\n" +
		 "You can go in any direction.\n");
	else
	set_noshow_obvious(1);
		set_long("With great skill you take your ship through.\n" +
		 "an asteroid belt.\n" +
		 "You can go in any direction.\n");
    }


	

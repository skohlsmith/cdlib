/*
   /d/Standard/login/ghost_player

   This player file is used when a player logging in does not have a valid
   player file.


      There can be three different reasons for not having a player_file:
      
         1 - If this is a new character, then manage the 
             creation / conversion / process.

         2 - The players racefile is not loadable, a new body must be
             choosen.

         3 - The players racefile is not a legal playerfile, a new body
             must be choosen.
*/
inherit "/std/player_pub";

#include <std.h>
#include <macros.h>
#include <stdproperties.h>
#include "/config/login/login.h"

static 	object		wiz,		/* For 'jr' this is the wizard */
                        pold;   	/* An 'old' player object */
static	string		wizpass;	/* The wizards password */

/*
 * Prototypes
 */
public void ghost_start();
void next_query();
void ask_player();

/*
 * Description:  Gives the object pointer to the old player to be converted
 */
public object query_old_player() { return pold; }

/*
 * Function name:   legal_player
 * Description:     This is called from the master object to check that
 *                  a specific player object is legal. This is used to
 *		    decide if restore_object / save_object in /players
 *		    is possible.
 * Arguments:       ob: The object to check
 * Returns:         True for legal player object
 */
nomask public int
legal_player(object ob)
{
    string m;
    mapping r;

    m =  MASTER_OB(ob);

    if (m == MASTER)
	return 1;

    r = RACEMAP;
    if (member_array(m, m_values(r)) >= 0)
	return 1;

    return 0;
}

/*
 * Function name:  enter_new_player
 * Description:    This is the function called from the login object.
 *                 It is responsible for initialising the correct actions
 *		   to be performed.
 */
public nomask void
enter_new_player(string name, string pass)
{
    if (!interactive(this_object()))
    {
	write("I'm linkdead, bye bye!\n");
	remove_object();
	return;
    }

    /*
     * Prevent hacking into (more) privileged accounts
     */
    if (!previous_object() || (MASTER_OB(previous_object()) != "/secure/login"
	&& MASTER_OB(previous_object()) != MASTER))
    {
	remove_object();
	return;
    }

    set_name(name);
    seteuid(0);

    /*
     * Check if the player exist
     */
    if (!SECURITY->load_player())
    {
	seteuid(getuid(this_object()));
	set_password(pass);
	set_player_file(MASTER);

	if (file_size(CONVERT_OLD) > 0)
	{
	    pold = SECURITY->load_old_player(read_file(CONVERT_OLD));
	    if (pold)
	    {
		set_mailaddr(pold->q_mailaddr());
		
		/* 
		 * Check so that the old password equals what was given
                 */
		if (pass != pold->q_password())
		{
		    write("\nWrong password!\nBye Bye.\n");
		    log_file("OLD_FAIL", ctime(time()) + " FAIL: " +
			     query_name() + " " + pass + ":" +
			     pold->q_password() +
			     " (" + query_ip_number() + ")\n");
		    destruct();
		    return;
		}
	    }
	}
		
	/*
         * name'jr' is a wizard helper if it is not an old player
         */
	if (!pold && extract(name, strlen(name) - 2, strlen(name)) == "jr")
	{
	    write("\nOk, you say you're a wizard. Let's see if it's true.\n" +
		  "And do remember: Max two helpers per wizard!\n");
	    write("\nInput your wizard name: ");
	    input_to("check_identity1", 0);
	    call_out("time_out", 120);
	    return;
	}

	/*
         * Is the mud open to new players ?
         */
#ifdef LOGIN_NO_NEW
	if (file_size(LOGIN_NO_NEW) > 0)   
	{
	    cat(LOGIN_NO_NEW);
	    log_file("REFUSED_ENTRY", ctime(time()) + ": " + name + "\n");
	    if (pold)
		pold->remove_object();
	    remove_object();
	    return;
	}
#endif

	if (pold)
	{
	    write("You have an old character of level: " + pold->q_level() +
		  ", that will be converted.\n");
	    set_ghost(GP_NEW | GP_CONVERT);
	}
	else
	{
	    write("Creating a new player.\n");
	    log_file("CREATE_PLAYER", ctime(time()) + " " + query_name() +
		     "(" + query_ip_number() + ")\n");
	    set_ghost(GP_NEW);
	}
	cat(LOGIN_FILE_NEW_PLAYER_INFO);

	ask_player();  /* Some questions are still needed */
	return;
    }
    seteuid(getuid(this_object()));

    /*
     * Is this a normal CDlib player with a bad playerfile ?
     */
    if (query_player_file() != MASTER)
    {
	set_ghost(GP_BODY | GP_DEAD);
	set_player_file(MASTER);
	write("You have a bad body and will have to choose a new.\n");
    }
    /*
     * Is it a new player with an old conversion character ?
     * This can be the case if the player 'quit' in the construction /
     * conversion process
     */
    else if (query_ghost() & GP_CONVERT)
    {
	if (file_size(CONVERT_OLD) > 0)
	    pold = SECURITY->load_old_player(read_file(CONVERT_OLD));
	if (!pold)
	    set_ghost(query_ghost() & ~GP_CONVERT);
    }
    ghost_start();
}

/*
 * Get the name of the old wizard and check out the identity.
 */
static void
check_identity1(string id)
{
    remove_call_out("time_out");

    wiz = (object)SECURITY->finger_player(id);

    if (!wiz || !wiz->query_wiz_level())
    {
	write(capitalize(id) + " isn't a wizard.\n");
	remove_object();
	return;
    }
    wizpass = (string)wiz->query_password();
    write("Input your wizard password: ");
    input_to("check_identity2", 1);
    call_out("time_out", 120);
    return;
}

/*
 * Get the password of the old wizard.
 */
static void
check_identity2(string p)
{
    write("\n");
    remove_call_out("time_out");
    if (crypt(p, wizpass) != wizpass)
    {
	write("Wrong password!\n");
	remove_object();
	return;
    }
    log_file("HELPER", "" + ctime(time()) + ": " +
	     capitalize(((string)wiz->query_real_name())) + " -> " +
	     query_name() + "\n");

    set_mailaddr(wiz->query_mailaddr());
    wiz->remove_object();
    set_ghost(GP_NEW);
    ghost_start();
}

void
time_out()
{
    if (pold)
	SECURITY->do_debug("destroy", pold);
    if (wiz)
	SECURITY->do_debug("destroy", wiz);

    write("Time out\n");
    remove_object();
}

/*****************************************************************
 *
 * The questions to ask an entirely new player, which is not handled
 * in the configuration process.
 *
     Ask for email adress

 */
static string   *new_queries;

/*
 * Function name: ask_player
 * Description:   Ask some questions of new players
 */
static void
ask_player()
{
    new_queries = ({ "dummy", "q_mail" });
    next_query();
    return;
}

/*
 * Function name: end_query
 * Description:   
 * Return:        
 */
static void
end_query() { ghost_start(); }

/*
 * Function name: next_query
 * Description:   Asks the next question of the user interactively.
 */
void
next_query()
{
    remove_call_out("time_out");
    while (1)
    {
	if (sizeof(new_queries) < 2)
	    return end_query();	/* does not return */
	new_queries = slice_array(new_queries, 1, sizeof(new_queries));
	if (call_other(this_object(), new_queries[0] + "_pretext"))
	{
	    call_out("time_out", 120);
	    input_to(new_queries[0]);
	    return;
	}
    }
}

/*
 * Function name: again_query
 * Description:   Asks the same question again.
 */
static void
again_query()
{
    if (call_other(this_object(), new_queries[0] + "_pretext"))
    {
	input_to(new_queries[0]);
	return;
    }
    next_query();
}


static int
q_mail_pretext()
{
    /*
     * Do not ask if there is already an email
     */
    if (query_mailaddr())
	return 0;
    write("Please enter your email address (or 'none'): ");
    return 1;
}

/*
 * Function:    q_mail
 * Description: This function is called using input_to, and sets the
 *              email adress of this player.
 */
static void
q_mail(string maddr)
{
    set_mailaddr(maddr);
    next_query();
}

/*
 * Here we start the actual configuration routine
 *
 * We have 'pold' as objectpointer to the old character to convert
 */
public void
ghost_start()
{	
    remove_call_out("time_out");
    if (!query_race_name())
    {
	set_race_name("newbie");
	save_me(0);
    }
    if (this_player() != this_object())
	set_this_player(this_object());

    /*
	We now have a correct .o file
    */
    write("\nEntering the hall of the bodies in waiting...\n\n");
    enter_game(query_real_name());
}

start_player()
{
    if (!sizeof(this_object()->query_cmdsoul_list()))
    {
	this_object()->add_cmdsoul("/d/Standard/cmd/soul_cmd_ghost");
	this_object()->add_cmdsoul("/d/Standard/cmd/misc_cmd_ghost");
    }
    ::start_player();
}

public string
query_default_start_location()
{
    if (query_ghost() & GP_BODY)
	return "/d/Standard/login/bodies";
    else if (query_ghost() & GP_MANGLE)
	return "/d/Standard/login/mangle";
    else if (query_ghost() & GP_FEATURES)
	return "/d/Standard/login/features";
    else if (query_ghost() & GP_SKILLS)
	return "/d/Standard/login/skills";
    else if (query_ghost() == 0)
	return RACESTART[this_object()->query_race_name()];

    /*
     * Should not happen
     */
    return "/d/Standard/login/bodies";
}

/*
 * Function name: stats_to_acc_exp
 * Description:   Translates the current base stats into acc_exp. This is used
 *                used when getting stats from a body.
 */
public nomask void
stats_to_acc_exp()
{
    ::stats_to_acc_exp();
}

public void set_exp(int e) { ::set_exp(e); }
public void set_exp_combat(int e) { ::set_exp_combat(e); }
public void update_acc_exp() { ::update_acc_exp(); }

/*
 * Now the configuration is ready, and we want to swap to the correct
 * playerfile. 
 */
public int
ghost_ready()
{
    string plfile;
    object ob;
    int wizlev;

    if (query_ghost())
    {
	write("You still have things to do before entering the world.\n");
	return 0;
    }
    plfile = RACEMAP[query_race_name()];
    if (!plfile)
    {
	write("You cannot be a " + query_race_name() + 
	      ", choose a new body!\n");
	set_ghost(GP_BODY | GP_DEAD);
	enter_new_player(query_real_name(), query_password());
	return 0;
    }

    ob = clone_object(plfile);
    if (!ob)
    {
	write(capitalize(query_race_name()) + 
	      " is a faulty race. Choose a new body!\n");
	set_ghost(GP_BODY | GP_DEAD);
	enter_new_player(query_real_name(), query_password());
	return 0;
    }	
    
    /* 
     * Prepare the new player object. We must change:
     *      We should have the correct souls.
     *      We should have the correct playerfile.
     */
    exec(ob, this_object());
    set_player_file(plfile);
    this_object()->transmsg_reset();
    set_default_start_location(RACESTART[this_object()->query_race_name()]);
    save_me(0);

    /*
     * Enter the game and load the save file
     */
    set_this_player(ob);
    if (!ob->enter_game(query_real_name()))
    {
	write(capitalize(query_race_name()) + 
	      " is an illegal race. Choose a new body!\n");
	set_ghost(GP_BODY | GP_DEAD);
	enter_new_player(query_real_name(), query_password());
	return 0;
    }

    /*
     * If it is a wizard, restore wizhood
     */
    if (wizlev = (int) (SECURITY->wiz_level(ob->query_real_name())))
    {
	write("You appear to be a wizard, now how did you die?\n");
	write("Restoring your immortality...\n\n");
	ob->set_wiz_level(wizlev);
    }
    ob->update_hooks();
    ob->save_me(0);

    /*
     * Now if we hammer hard enough it probably goes away
     */
    call_out("damn_stubborn_object",1);
    remove_object();
    destruct();
}

void damn_stubborn_object()
{
    call_out("damn_stubborn_object",1);
    destruct();
}    
    
/*
 * Function name: reincarnate_me
 * Description:   Called by a player object that is a ghost and needs to
 *		  be reincarnated.
 */
public void
reincarnate_me()
{
    object pl, gh, n, p;

    pl = previous_object();
    if (!interactive(pl) || !pl->query_ghost())
	return;

    pl->set_ghost(GP_BODY | GP_DEAD);
    pl->set_temp_start_location(0);
    pl->save_me(0);

    gh = clone_object(MASTER);
    exec(gh, pl);

    gh->set_bit_array(pl->query_bit_array());
    n = pl->query_real_name();
    SECURITY->do_debug("destroy", pl);
    gh->enter_new_player(n);
}

query_race()
{
    return "ghost";
}

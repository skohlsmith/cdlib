/* bodies.c
   Tintin 920106 */

inherit "/std/room";

#include <stdproperties.h>
#include <cmdparse.h>
#include <ss_types.h>
#include "/config/login/login.h"

mapping bodies;

/*
 * Function name: create_room
 * Description  : Create the room.
 * In this function, we declare everything that should be fixed
 * when the room is started, and not changed in resets.
 */

create_room()
{
    set_short("Hologram room");
    set_long("You are in an small room. The room is almost dark, the only\n" +
	     "illumination comming from several holograms around you.\n" +
	     "Each hologram is a picture of a body. There is an iris hatch at one\n" +
	     "end of the hall. Above the hatch there is a sign saying:\n" +
	     "               TRANSMOGRIFYER\n");
    bodies = ([ ]);
    add_prop(ROOM_I_INSIDE, 1);
    call_out("make_one_of_each", 1, sizeof(RACES) - 1);
}

void
make_one_of_each(int j)
{
    int i;
    string tmp;
    object ob;

    for (i = 0; i < 2; i++)
    {
	tmp = RACES[j] + (i ? "male" : "female");
	bodies[tmp] = 1;
	ob = clone_object(PATH + "body");
	ob->create_body(RACES[j], (i ? "male" : "female"));
	ob->move(this_object());
    }
    if (j == 0)
	call_out("make_random", 1, 4);
    else
	call_out("make_one_of_each", 1, j - 1);
}

void
make_random(int j)
{
    string tmp;
    object ob;

    ob = clone_object(PATH + "body");
    ob->create_body();
    ob->move(this_object());

    tmp = ob->query_race_name() + ob->query_gender_string();
    bodies[tmp]++;

    if (j)
	call_out("make_random", 1, j - 1);
}

/*
 * Function name: init
 * Description  : Add commands etc.
 */
public void
init()
{
    add_action("all_cmd", "", 1);
}

/*
 * Function name: long
 * Description  : Modify the behaviour of long
 */
public varargs mixed
long(string arg)
{
    mixed str;

    str = ::long(arg);

    if (arg)
	return str;

    if (this_player()->query_ghost() & GP_BODY)
	str += "\nSince the Klingons blasted your spaceship and most of you\n" +
	    "to pieces and the medics were out of spare parts you are here to\n" +
		"find your self a body suitable for your next mission.\n\n" +
	    "Perhaps you should examine them, select a body and enter it.\n";
    else
	str += "\nThere are thousands of other unfortunate people waiting\n" +
	    "to get a new body. You should probably leave as fast as you can.\n\n";
    str += "\nEach hologram includes a flashing number above the body.\n";
    return str;
}
/*
 * Function name: update_player
 * Description  : Modify the behaviour of long
 */
nomask void
update_player(object ob)
{
    object player, oplayer;
    int deadness;
    int *skill_types, il, val;


    player = this_player();
    deadness = player->query_ghost();

/*    Case 1: Old player died, he selects body of same race and gender
	Give him his body, appearance and send him packing
      Case 2: old player died. He selects body of different race &| gender
	Give him new race, appearance and gender, take away half of each
        skill he has, set that he should go through features and mangle
      Case 3: New player enters the game
        Give him a body, set stats, race, appearance and Gender
      Case 4: Conversion of old type player
        Suck out exp. Set stats, race, gender and appearance.

	    */
#define OLD_NUMQUESTS 12

    player->set_appearance(ob->query_appearance());

    if(deadness & GP_DEAD)
    {
	if (player->query_race_name() == ob->query_race_name() &&
	    player->query_gender() == ob->query_gender())
	{      /* Case 1 */
	    player->set_ghost(0);
	    player->ghost_ready();
	}
	else
	{
	    /* We should modify stats to reflect base stats
	     * but this can wait until later
	     */

	    /* Set skills to half
	     */
	    tell_object(player,"You will be less skilled, as this is a different kind of body than your last.\n");
	    skill_types = player->query_all_skill_types();
	    for (il = 0; il < sizeof(skill_types); il++)
	    {
		val = player->query_skill(skill_types[il]);
		player->set_skill(skill_types[il], val / 2);
	    }

	    player->set_race_name(ob->query_race_name());
	    player->set_gender(ob->query_gender());
	    player->set_ghost((player->query_ghost() | GP_MANGLE |
			      GP_FEATURES) - GP_BODY);
	}
    }
    else
    {
	player->set_race_name(ob->query_race_name());
	player->set_gender(ob->query_gender());
	player->set_ghost(player->query_ghost() & ~GP_BODY);

	for (il == SS_STR; il <= SS_DIS; il++)
	    player->set_base_stat(il, ob->query_base_stat(il));

	/*
         * Old players should be allowed to set their prefs later on
         */
	player->set_learn_pref(0);

	player->stats_to_acc_exp();

	player->set_opinion(random(100));

	if (deadness & GP_CONVERT) /* Case 4 */
	{
	    /* Suck out quests and old exp */
	    /* Use them to modify player exp */
	    oplayer = player->query_old_player();
	    if (!oplayer)
	    {
		write("Can't find your old player!\n");
	    }
	    else
	    {
		string quests;
		int exp, cexp, nq;
		quests = oplayer->q_quests();
		exp = oplayer->q_exp();
		if (exp < 0)
		    exp = 0;
		if (exp > 1000000)
		    exp = 1000000 + (exp - 1000000) / 987;
		if (strlen(quests))
		    nq = sizeof(explode(quests,"#"));
		else
		    nq = 0;
		if (nq > OLD_NUMQUESTS)
		    nq = OLD_NUMQUESTS;
		cexp = exp * (OLD_NUMQUESTS - nq) / OLD_NUMQUESTS;
		player->set_exp_general(3*exp);
		player->set_exp_combat(3*cexp);
		player->update_acc_exp();
	    }
	}
	else /* Case 3 */
	{
	    /* This block intentionally left blank */
	}
	player->set_hp(10000);
	player->set_mana(10000);
	player->set_fatigue(10000);
    }

}

/*
 * Function name: enter_cmd
 * Description  : Resolve the different enter commands
 */
public void
enter_cmd(string str)
{
    object player, *a;
    string tmp;

    if (!str || !strlen(str))
    {
	write("Enter what?\n");
	return;
    }

    player = this_player();
    if (str == "hatch")
    {
	if (player->query_ghost() & GP_BODY)
	    write("You signed a contract with Star Burgers letting them have\n" +
		  "your old body. They won't let you out until you fullfill\n" +
		  "the contract.\n");
	else if (!player->query_ghost())
	    player->move_living(" through the iris hatch.",
				player->query_def_start());
	else
	{
	    write("As you enter the iris hatch, it closes behind you.\n" +
		  "You are now in the imperial center of transmogrofy.\n\n");
	    player->move_living(" through the iris hatch.", PATH + "mangle");
	}
	return;
    }

    a = CMDPARSE_ONE_ITEM(str, "enter_body", "enter_access");
    if(!sizeof(a))
    {
	write("I can't find any " + str + " to enter.\n");
	return;
    }
    if (sizeof(a) > 1)
    {
	write("You have to be more specific.\n");
	return;
    }

    if(!(player->query_ghost() & GP_BODY))
    {
	write("Getting another body? You sure can't afford that.\n");
	return;
    }
    write("As you enter the number on the keyboard a pink beam\n" +
	  "hits you from above. The light blinds you, and you\n" +
	  "lose control of your remaining limbs.\n");
    say(capitalize(a[0]->query_real_name()) +
	" flickers brighly and walks out of the hologram.\n");
    update_player(a[0]);
    tmp = a[0]->query_race_name() + a[0]->query_gender_string();
    if (bodies[tmp]-- == 1)
	call_out("create_new", 1);
    else
	call_out("create_new", 1,
		 ({a[0]->query_race_name(), a[0]->query_gender_string() }) );
    a[0]->remove_object();
}

int
enter_access(object ob)
{
    return (function_exists("create_creature", ob) == (PATH + "body"));
}

int enter_body(object ob) { return 1; }

varargs void
create_new(string *a)
{
    object ob;
    string tmp;

    ob = clone_object(PATH + "body");
    if (a)
	ob->create_body(a[0], a[1]);
    else
	ob->create_body();

    ob->move(this_object());

    tmp = ob->query_race_name() + ob->query_gender_string();
    bodies[tmp]++;
    tell_room(this_object(), "You hear a ringing in your ears as your\n" +
	      "vision returns to normal. You see your old crippled\n" +
	      "lying on the floor in front of the keyboard. Suddenly\n" +
	      "a hatch in the floor opens and the body falls through it.\n" +
	      "As you take a step forward with your new body, a new\n" +
	      "hologram pops up.\n");
}

/*
 * Function name: all_cmd
 * Description  : Catch all player commands. Take care of the legal ones
 *                and throw all the others away
 */
public int
all_cmd(string str)
{
    switch(query_verb())
    {
    case "arrive":
    case "glance":
    case "depart":
    case "say":
    case "quit":
    case "Goto":
    case "goto":
    case "look":
    case "exa":
    case "examine":
    case "compare":
    case "sysbug":
    case "home":
    case "trans":
	return 0;
	break;
    case "enter":
    case "select":
	enter_cmd(str);
	return 1;
	break;
    case "exit":
    case "leave":
	enter_cmd("hatch");
	return 1;
	break;
    case "help":
	write("You are supposed to choose a body to replace your defect body.\n");
	write("You can look at the bodies with for example, 'look at #25'\n");
	write("You can compare a stat for two bodies by, for example:\n");
	write("   'compare str of #25 and #30'\n");
	write("There is six stats to compare: str, con, dex, int, wis, dis\n");
	write("When you have choosen, you do: 'enter #num'\n");
	return 1;
	break;
    default:
	if (this_interactive()->query_wiz_level() > 40)
	    return 0;
	write("That is not possible here.\n");
	return 1;
    }
}

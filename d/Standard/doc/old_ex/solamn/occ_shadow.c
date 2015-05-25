/*
 * This is the autoloading shadow for the Solamnian Knight guild.
 *
 * Made by Nick
 */

inherit "/std/guild/guild_occ_sh";
#include "/d/Krynn/common/defs.h"
#include "../guild.h"
#include <ss_types.h>
#include <language.h>

#define SOL_SUB "solamnian_subloc"

/*
 * Prototypes
 */
int query_level_up(int stat, int level);
int query_level_down(int stat);

/* Function name: query_guild_tax
 * Description:   Set the initial tax
 */
int query_guild_tax_occ() { return 13; }

/* Function name: query_guild_keep_player
 * Description:   Test if we want to keep the player in the guild.
 *		  If we discover a goblin we will keep him, just to throw him
 *		  out the proper way later.
 * Arguments:     ob - the player.
 * Returns:	  1 if keeping, 0 if not.
 */
int
query_guild_keep_player(object ob)
{
    ob->add_subloc(SOL_SUB, this_object());

    if (ob->query_race_name() == "goblin")
    {
	write("We don't like goblins in the Solamnian Guild.\n");
	call_out("goblin_leave", 10, ob);
    }
    return 1;
}

void goblin_leave(object ob) { ADMIN->goblin_leave(ob); }

/* Function name: query_guild_not_allow_join_occ
 * Description:	  Check if we allow that the player joins another guild.
 * Arguments:     player - the player
 *		  type, style, name - the type, style and name of the
 *			intended guild.
 * Returns:	  1 if we do not allow, else 0
 */
int
query_guild_not_allow_join_occ(object player, string type, string style,
				string name)
{
    if (::query_guild_not_allow_join_occ(player, type, style, name))
        return 1;

    NF("We brave fighters don't mess with magic.\n");
    if (style == "magic")
        return 1;

    NF("We brave fighters don't want thieves in our ranks.\n");
    if (style == "thief")
        return 1;

    return 0;
}

/* Function name: query_knight_level
 * Description:   What level have the knight achieved in the Knighthood ?
 * Returns:	  the level
 */
int
query_knight_level()
{
    object ob;

    if (ob = present("pal_med", find_player(lower_case(this_object()->
			query_name()))))
	return ob->query_knight_level();
    return 0;
}
    
/* Function name: query_def_post
 * Description:   We change the default post office for our members.
 * Returns:	  The new postoffice
 */
string query_def_post() { return "/d/Krynn/solace/guild/post"; }

/* Function name: query_knight_title
 * Description:   What is the title of this knight ?
 * Returns:	  The Knight title
 */
string
query_knight_title() { return KNIGHT_TITLES[query_knight_level()]; }

/* Function name: query_title
 * Description:   We shadow the title function so knights can have titles of
 *		  their own.
 * Returns:	  The completet title.
 */
string
query_title()
{
    string title, my;

    title = shadow_who->query_title();
    my = query_knight_title();

    if (shadow_who->query_wiz_level())
	return title;
    if (strlen(title))
	return title + " and " + my;
    return my;
}

/* Function name: query_guild_skill_name
 * Description:   When a players uses the skills command he should get the
 *		  the skill names, not the 'special' string.
 * Arguments:     type - the number of the skill
 * Returns:	  0 if no skill of mine, else the string.
 */
mixed
query_guild_skill_name(int type)
{
    if (type >= SS_PRESTIGE_ACC && type <= SS_PRESTIGE_ST)
	return "";

    if ((type < SS_RESCUE) || (type > SS_KICK))
	return 0;

    return KNIGHT_SKILL_DESC[type - SS_RESCUE];
}

/* Function name: q_duel and q_att_me
 * Description:	  Will later keep track of players that you have offered to
 *		  dueal with and players that have attacked you.
 * Returns:	  array of strings - their names
 */
string *q_duel() { return ({}); }
string *q_att_me() { return ({}); }

/* Function name: query_ob_prestige
 * Description:   Check for the prestige of the object.
 * Arguments:     ob - the object to look at
 * Returns:	  pres - the prestige
 */
int
query_ob_prestige(object ob)
{
    int pres;

    pres = ob->query_knight_prestige();
    if (!ob->query_npc() && (member_array(ob->query_real_name(),
			q_duel() + q_att_me()) == -1))
    {
	if (ob->query_race_name() == "goblin")
	    pres = 0; /* No big deal killing a goblin player */
	else
	    pres = -5;
    }
    if (!pres)
    {
	if (ob->id("gargoyle"))
	    pres = 50;
	else if (ob->id("goblin"))
	    pres = 40;
	else if (ob->id("dragon"))
	    pres = 400;
    }

    return pres;
}

/* Function name: notify_you_killed_me
 * Description:	  This is called each time the knight kills something.
 * Arguments:     ob - the object that was killed
 */
void
notify_you_killed_me(object ob)
{
    int pres;

    if (living(ob) && !(ob->query_npc()))
	KILLED(shadow_who, ob);

    call_out("change_prestige", 1, query_ob_prestige(ob));
}

/* Function name: change_prestige
 * Description:   When a player has killed something or should change the 
 *		  prestige for some other reason, call this function.
 * Arguments:     pres - the prestige to add.
 */
void
change_prestige(int pres)
{
    int level, new_level;
    object ob;

    if (!(ob = present("pal_med", find_player(lower_case(this_object()->
                        query_name())))))
        return;

    level = query_knight_level();

    if (pres > 0)
    {
	if (level == L_BROSE)
	    pres = 2 * pres / shadow_who->query_average_stat();
	shadow_who->set_skill(SS_PRESTIGE_ACC,
			      shadow_who->query_skill(SS_PRESTIGE_ACC) +
			      pres);
	shadow_who->set_skill(SS_PRESTIGE_ST, 
			      shadow_who->exp_to_stat(
				   shadow_who->query_skill(SS_PRESTIGE_ACC)));
	new_level = query_level_up(shadow_who->query_skill(SS_PRESTIGE_ST),
				   level);
    } else if (pres == 0) {
	shadow_who->set_skill(SS_PRESTIGE_ACC,
		shadow_who->query_skill(SS_PRESTIGE_ACC) - level);
	shadow_who->set_skill(SS_PRESTIGE_ST, shadow_who->exp_to_stat(
	    shadow_who->query_skill(SS_PRESTIGE_ACC)));
	new_level = query_level_down(shadow_who->query_skill(SS_PRESTIGE_ST));
    } else {
	shadow_who->set_skill(SS_PRESTIGE_ST,
		shadow_who->query_skill(SS_PRESTIGE_ST) + pres);
	shadow_who->set_skill(SS_PRESTIGE_ACC, shadow_who->stat_to_exp(
	    shadow_who->query_skill(SS_PRESTIGE_ST)));
	new_level = query_level_down(shadow_who->query_skill(SS_PRESTIGE_ST));
    }

    if (level > new_level)
    {
	shadow_who->catch_msg(break_string("Paladin doesn't agree with " +
	    "your actions. Your prestinge have lowered too much. Your rank " +
	    "has lowered.\n", 70));
	ob->set_knight_level(new_level);
	ob->set_tax(1);
    } else if (level < new_level) {
	shadow_who->catch_msg(break_string("Your prestige has raised to " +
	    "enough to let you get a higher rank. Congratulations.\n", 70));
	ob->set_knight_level(new_level);
	ob->set_tax(0);
    }
}

int
query_level_up(int stat, int level)
{
    if (stat > SS_UROSE)
	return L_ROSE;
    if (stat > SS_USHIELD)
    {
	if (level == L_ROSE)
	    return L_ROSE;
	else
	    return L_SHIELD;
    }
    if (stat > SS_UCROWN)
    {
	if (level == L_SHIELD)
	    return L_SHIELD;
	else
	    return L_CROWN;
    }
    if (stat > SS_USQUIRE)
    {
	if (level == L_CROWN)
	    return L_CROWN;
	else
	    return L_SQUIRE;
    }
    if (stat >= SS_DBROSE)
	return L_SQUIRE;
    return L_BROSE;
}

int
query_level_down(int stat)
{
    if (stat < SS_DBROSE)
	return L_BROSE;
    if (stat < SS_DSQUIRE)
	return L_SQUIRE;
    if (stat < SS_DCROWN)
	return L_CROWN;
    if (stat < SS_DSHIELD)
	return L_SHIELD;
    return L_ROSE;
}

/*
 * Function name: show_subloc
 * Description:   This function is called each time someone looks at us
 * Arguments:     subloc  - Our subloc
 *		  me      - I
 *		  for_obj - The looker
 * Returns:	  The string the looker shall see
 */
string
show_subloc(string subloc, object me, object for_obj)
{
    string str;

    if (me->query_prop(TEMP_SUBLOC_SHOW_ONLY_THINGS))
	return "";

    if (subloc != SOL_SUB)
        return shadow_who->show_subloc(subloc, me, for_obj);

    if (for_obj == me)
	str = "You are ";
    else
	str = capitalize(me->query_pronoun()) + " is ";

    return str + "a Solamnian Knight.\n";
}

/*
 * Function name:
 * Description:
 * Arguments:
 * Returns:
 */

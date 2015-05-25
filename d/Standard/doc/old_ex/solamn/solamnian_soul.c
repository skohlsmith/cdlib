/*
 * Solamnian_soul.c
 *
 * This soul holds the general commands and feelings of the Solamnian Knights
 *
 */

inherit "/cmd/std/command_driver";

#include "/d/Krynn/common/defs.h"
#include "../guild.h"
#include "/d/Krynn/open/sys/combat.h"

#include <ss_types.h>
#include <macros.h>
#include <stdproperties.h>
#include <wa_types.h>
#include <cmdparse.h>
#include <filter_funs.h>
#include <composite.h>

/*****************************************************************************
 * Soul identifying and autoloading
 *****************************************************************************/

string get_soul_id() { return "Solamnian Knight"; }

int query_cmd_soul() { return 1; }

/*****************************************************************************
 * The list of verbs and functions. Please add new in alfabetic order.
 *****************************************************************************/

mapping
query_cmdlist()
{
    seteuid(getuid());
    return ([
	"agree":"agree",
	"bash":"bash",
	"ckick":"ckick",
	"guildlist":"guildlist",
	"help":"help",
	"honour":"honour",
	"leave!":"leave",
    ]);
}

static mixed
parse_this(string str, string form)
{
    object   *oblist, *tmplist;
    int      i;

    tmplist = ({});
    if (!CAN_SEE_IN_ROOM(this_player()))
        return tmplist;

    if (!str || (!parse_command(lower_case(str), environment(this_player()),
                         form, oblist)))
        return ({});

    oblist = NORMAL_ACCESS(oblist, 0, 0);

    if (!sizeof(oblist))
        return ({});

    for (i = 0; i < sizeof(oblist); i++)
    {
        if (objectp(oblist[i]) && living(oblist[i]) &&
                (this_player() != oblist[i]) &&
                CAN_SEE(this_player(), oblist[i]))
            tmplist = tmplist + ({ oblist[i] });
    }
    return tmplist;
}

/*****************************************************************************
 * Here follows the functions. New functions should be added in alfabetic
 * order.
 ****************************************************************************/

/*
 * Global variables
 */

int kick_again;		/* Check this if possible to kick again. */
int bash_again;		/* Check this if possible to bash again. */

/********************* agree ************************************************/
int
agree(string str)
{
    object *ob;

    if (!str)
    {
	write("You agree.\n");
	say(QCTNAME(TP) + " agrees.");
	return 1;
    }

    ob = parse_this(str, "[with] %l");
    NF("Agree with whom?\n");
    if (!sizeof(ob))
	return 0;
    write("You agree with " + QTNAME(ob[0]) + ".\n");
    ob[0]->catch_msg(QCTNAME(TP) + " agrees with you.\n");
    say(QCTNAME(TP) + " agrees with " + QTNAME(ob[0]) + ".\n", ({ ob[0] }));
    return 1;
}

/********************* bash *************************************************/
int
bash(string str)
{
    object ob, *obj;
    mixed *hitresult;
    string how;

    if (!str)
        ob = TP->query_attack();
    else {
	obj = parse_this(str, "%l");
  	if (sizeof(obj) > 0)
	    ob = obj[0];
	if (sizeof(obj) > 1)
	{
	    NF("Choose one enemy to bash.\n");
	    return 0;
	}
    }

    how = c_can_attack(ob, query_verb());
    if (stringp(how))
    {
	NF(how);
	return 0;
    }

    if (!TP->query_prop(KNIGHT_I_BASH))
    {
	TP->add_prop(KNIGHT_I_BASH, 3);
	call_out("bash_ob", 3, ({ ob, TP }));
	TP->catch_msg("You prepare a bash on " + QTNAME(ob) + ".\n");
	if (ob != TP->query_attack())
	{
	    say(QCTNAME(TP) + " looks at " + QTNAME(ob) + " with murder " +
		"in " + TP->query_possessive() + " eyes.\n", ({ ob, TP }));
	    ob->catch_msg(QCTNAME(TP) + " looks at you with murder in " +
		TP->query_possessive() + " eyes.\n");
	}
    } else if (TP->query_prop(KNIGHT_I_BASH) > 1)
	write("You are already preparing another bash.\n");
    else
	write("You have to get yourself together from the earlier " +
		"bash before.\n");
    return 1;
}

void
bash_ob(object *ob)
{
    string how;

    set_this_player(ob[1]);
    how = c_can_attack(ob[0], "bash");
    if (stringp(how))
    {
	write("You are unable to bash the intended victim.\n");
	TP->remove_prop(KNIGHT_I_BASH);
	return;
    }

    call_out("allow_bash", 15, TP);

    if (ob[0] && environment(TP) != environment(ob[0]))
        return;

    TP->add_prop(LIVE_O_SPELL_ATTACK, this_object());
    TP->attack_object(ob[0]);
    TP->add_prop(KNIGHT_I_BASH, 2);
}

void
allow_bash(object ob) { ob->remove_prop(KNIGHT_I_BASH); }

/******************************** Special attacks end up here ****************/
void
spell_attack(object attacker, object target)
{
    string how;
    mixed *hitresult;

    if (attacker->query_prop(KNIGHT_I_BASH) == 2)
    {
	attacker->add_prop(KNIGHT_I_BASH, 1);
	if (random(F_BASH_HIT(attacker, target)) > random(BASH_HIT))
        {
            hitresult = target->hit_me(F_BASH_PEN(attacker, target),
		W_BLUDGEON, attacker, -1;
	    how = "unharmed though";
	    if (hitresult[0] > 0)
	    	how = "a little hurt";
	    if (hitresult[0] > 5)
	    	how = "rather hurt";
	    if (hitresult[0] > 10)
		how = "shaken by the experience";
	    if (hitresult[0] > 25)
		how = "extremely hurt";
	    if (hitresult[0] > 50)
		how = "like hit buy a hurricane";

            attacker->catch_msg("You send " + QTNAME(target) +
		" sprawling with a wonderful bash.\n");
	    attacker->catch_msg(QCTNAME(target) + " looks " + how + ".\n");

            target->catch_msg(QCTNAME(attacker) +
		" sends you sprawling with a bash.\n");
	    target->catch_msg("You feel " + how + ".\n");

	    tell_watcher(QCTNAME(attacker) + " sends " + QTNAME(target) +
		" sprawling with a wonderful bash.\n", attacker, target);

	    if (target->query_hp() <= 0)
	        target->do_die(attacker);
            return;
        }

    	attacker->add_prop(LIVE_I_ATTACK_DELAY, 2);
    	target->hit_me(0, W_BLUDGEON, attacker, -1);
    	attacker->catch_msg("You try to bash " + QTNAME(target) +
	    " but you miss.\n" + "You stumble and fall to the ground.\n");
    	target->catch_msg(QCTNAME(attacker) +
	    " tries to bash you but misses.\n" + C(attacker->query_pronoun()) +
	    " stumbles and falls.\n");
	tell_watcher(QCTNAME(attacker) + " tries to bash " + QTNAME(target) +
	    " but stumbles and falls.\n", attacker, target);
    	return;
    }
    else if (attacker->query_prop(KNIGHT_I_KICK) == 2)
    {
        attacker->add_prop(KNIGHT_I_KICK, 1);
        if (random(F_BASH_HIT(attacker, target)) > random(CKICK_HIT))
        {
            hitresult = target->hit_me(F_CKICK_PEN(attacker, target),
		W_BLUDGEON, attacker, -1);
            how = "without effect";
            if (hitresult[0] > 0)
                how = "hard";
            if (hitresult[0] > 5)
                how = "rather hard";
            if (hitresult[0] > 10)
                how = "very hard";
            if (hitresult[0] > 25)
                how = "extremely hard";
            if (hitresult[0] > 50)
                how = "with a bonecrashing sound";

            attacker->catch_msg("You kick " + QTNAME(target) +  " in " +
                target->query_possessive() + " " +
                hitresult[1] + " " + how + ".\n");
            target->catch_msg(QCTNAME(attacker) + " kicks you " + how +
		" in your " + hitresult[1] + ".\n");
	    tell_watcher(QCTNAME(attacker) + " kicks " + QTNAME(target) +
		" " + how + ".\n", attacker, target);

            if (target->query_hp() <= 0)
                target->do_die(attacker);
            return;

        }

	attacker->add_prop(LIVE_I_ATTACK_DELAY, 2);
        target->hit_me(0, W_BLUDGEON, attacker, -1);
        attacker->catch_msg("You try to kick " + QTNAME(target) +
            " but you miss.\n" + "You stumble and fall to the ground.\n");
        target->catch_msg(QCTNAME(attacker) +
            " tries to kick you but misses.\n" + C(attacker->query_pronoun()) +
            " stumbles and falls.\n");
	tell_watcher(QCTNAME(attacker) + " tries to kick " + QTNAME(target) +
	    " but stumbles and falls.\n", attacker, target);
        return;
    }
}

/********************* ckick ************************************************/
int
ckick(string str)
{
    object ob, *obj;
    mixed *hitresult;
    string how;

    if (!str)
        ob = TP->query_attack();
    else {
        obj = parse_this(str, "%l");
        if (sizeof(obj) > 0)
            ob = obj[0];
        if (sizeof(obj) > 1)
        {
            NF("Choose one enemy to kick.\n");
            return 0;
        }
    }

    how = c_can_attack(ob, query_verb());
    if (stringp(how))
    {
        NF(how);
        return 0;
    }

    if (!TP->query_prop(KNIGHT_I_KICK))
    {
        TP->add_prop(KNIGHT_I_KICK, 3);
        call_out("ckick_ob", 3, ({ ob, TP }));
        TP->catch_msg("You prepare a combat kick on " + QTNAME(ob) + ".\n");
	if (ob != TP->query_attack())
	{
	    say(QCTNAME(TP) + " looks at " + QTNAME(ob) + " with murder " +
		"in " + TP->query_possessive() + " eyes.\n", ({ ob, TP }));
	    ob->catch_msg(QCTNAME(TP) + " looks at you with murder in " +
		TP->query_possessive() + " eyes.\n");
	}
    } else if (TP->query_prop(KNIGHT_I_KICK) > 1)
        write("You are already preparing another combat kick.\n");
    else
        write("You have to get yourself together from the earlier " +
                "kick before.\n");
    return 1;
}

void
ckick_ob(object *ob)
{
    string how;

    set_this_player(ob[1]);
    how = c_can_attack(ob[0], "kick");
    if (stringp(how))
    {
        write("You are unable to kick the intended victim.\n");
        TP->remove_prop(KNIGHT_I_KICK);
        return;
    }

    call_out("allow_ckick", 15, TP);

    if (ob[0] && environment(TP) != environment(ob[0]))
        return;

    TP->add_prop(LIVE_O_SPELL_ATTACK, this_object());
    TP->attack_object(ob[0]);
    TP->add_prop(KNIGHT_I_KICK, 2);
}

void
allow_ckick(object ob) { ob->remove_prop(KNIGHT_I_KICK); }

/********************* guildlist ********************************************/
int
guildlist(string str)
{
    string name;
    object ob;

    if (str)
    {
        if (TP->query_wiz_level())
        {
            name = C(str) + " is ";
            NF("No such player found.\n");
            if (!(ob = find_player(str)))
                return 0;
        } else {
            write("Curious aren't we?\n");
            return 1;
        }
    } else {
        ob = TP;
        name = "You are ";
    }

    str = ob->list_mayor_guilds();
    if (str)
        write(name + "member of following guilds.\n" + str);
    else
        write(name + "not member of any guilds.\n");

    return 1;
}

/********************* help *************************************************/
int
help(string str)
{
    string subject, filename;

    if (!str)
        return 0;

    if (sscanf(str, "guild %s", subject) != 1)
        return 0;

    seteuid(getuid(TO));
    filename = HELP_DIR + subject;
    if (file_size(filename) > 0)
    {
        cat(filename);
        return 1;
    }

    NF("No help on that subject I'm afraid.\n");
    return 0;
}

/********************* honour ***********************************************/
int
honour(string str)
{
    object *ob;
    int i, own_level;

    NF("A Knight of the Black Rose has no honour.\n");
    if ((own_level = TP->query_knight_level()) == L_BROSE)
	return 0;

    NF("Honour who?\n");
    ob = parse_this(str, "[the] %l");
    if (!sizeof(ob))
	return 0;

    for (i = 0; i < sizeof(ob); i++)
	if ((!ob[i]->query_wiz_level() &&
		(ob[i]->query_knight_level() < own_level)) ||
		TP->query_wiz_level())
	{
	    ob[i]->catch_msg(QCTNAME(TP) + " gives you " +
		TP->query_possessive() + " blessings.\n");
	    TP->catch_msg("You give " + QTNAME(ob[i]) + " your blessings.\n");
	} else {
	    ob[i]->catch_msg(QCTNAME(TP) + " bows deeply infront of you.\n");
	    TP->catch_msg("You bow deeply infront of " + QTNAME(ob[i]) + ".\n");
	}

    return 1;
}

/********************* leave ************************************************/
int
leave(string str)
{
    int i, num;

    NF("Leave what? The Solamnian Knights?\n");
    if (str != "The Solamnian Knights")
        return 0;

    write("You leave the Solamnian Knights.\n");
    say("The world cry when " + QTNAME(TP) + " leaves the Solamnian " +
        "Knights.\n");

    LEAVE(TP);

    TP->remove_cmdsoul(KNIGHT_SOUL);
    if (P("pal_med", TP))
	P("pal_med", TP)->remove_object();
    TP->update_hooks();

    return 1;
}


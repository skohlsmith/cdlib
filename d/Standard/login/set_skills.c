
/* 
 * This is an object that allows people to choose skills.
 */

#define PATH "/d/Standard/login/"

inherit "/std/object";
inherit "/lib/skill_raise";

#include "/config/login/login.h"
#include "/sys/stdproperties.h"
#include "/sys/ss_types.h"

public int done();

create_object() 
{
    set_name("trainer"); 
    set_short("skill trainer");

    add_prop(OBJ_I_NO_DROP, 1);
    set_no_show(1);
    
    create_skill_raise();

    sk_add_train(SS_PILOT	       ,0 ,0, 0, 32);
    sk_add_train(SS_WEP_ASSRIFLE           ,0 ,0, 0, 29);

}

init()
{
    add_action("ground", "ground");
    add_action("space", "space");
    add_action("instruct", "instructions");
    add_action("instruct", "help");
    add_action("instruct", "ask");
}

public int instruct(string str)
{
    write(
	  "The robot says: I will implant a knowledge chip in you. You may choose\n" + 
	  "'space' or 'ground' version of chip.\n\n");
    return 1;
}

public int
space()
{
    int sk;
    string *st;

    sk = sk_find_skill("pilot");
    sk_do_train(sk, this_player(), 50);
    write("The robot puts one of its 24 arms on the top of your head. You feel\n" +
	  "a small pain as it makes a small hole in your skull. Just a second\n" +
	  "later he says: There you are son, not worse than going to the dentist\n" +
	  "Grinning at you with his steel teeth he points at the teleporter.\n");
    done();

    return 1;
}

public int
ground()
{
    int sk;
    string *st;

    sk = sk_find_skill("assault rifle");
    sk_do_train(sk, this_player(), 50);
    write("The robot puts one of its 24 arms on the top of your head. You feel\n" +
	  "a small pain as it makes a small hole in your skull. Just a second\n" +
	  "later he says: There you are son, not worse than going to the dentist\n" +
	  "Grinning at you with his steel teeth he points at the teleporter.\n");
    done();

    return 1;
}
	
public int
done()
{
    if (this_player()->query_ghost() & GP_SKILLS)
	this_player()->set_ghost(this_player()->query_ghost() - GP_SKILLS);
    destruct();
    return 1;
}

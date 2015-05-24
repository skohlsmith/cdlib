/* death.c */
/* Mrpr 901120 */
/* Tintin 911031 */

inherit "/std/monster";
#include "/sys/stdproperties.h"

/*
 * Function name: init
 * Description:   Init Death
 */
init() 
{
    ::init();
    add_action("take_it", "take");
}

/*
 * Function name: reset
 * Description:   Reset Death
 */
void
create_monster() 
{
    set_name("figure");
    set_name("moot");
    set_name("death");
    set_race_name("immortal");
    set_adj("shadowlike");
    set_short("shadowlike figure, clad in black");
    add_prop(LIVE_I_ALWAYSKNOWN, 1);
}

/*
 * Function name: long
 * Description:   Long description
 */
long(str)
{
    
    if (str == "death" || str == "moot")
    {
	return
"Death seems to have taken Jane Fonda's exercise and diet program much too\n" +
"seriously. A clear case of Anorexia Neurosa. Except for a wicked looking scythe\n" +
"he's dressed in a black hooded robe that suits him admireably well. There's\n" +
"something about his eyes as well, or maybe the lack of eyes, that you \n" +
"feel you'd better not investigate too closely.\n";
    }
    
    if (str == "scythe")
    {
	return
"An extremly sharpened scythe. It's so sharp that gusts of wind actually try\n" +
"to turn away from the edge rather than be sliced in two by the wicked looking\n" +
"blade. It does strange things with light as well as unlucky photons split into\n" +
"their sub-components when they hit the blade.\n";
    }
    
    if (str == "robe")
    {
	return
"A black hooded robe with numerous pockets. It doesn't seem to fit you very\n" +
"well however. It seems to have been tailored for a very lean customer.\n" +
"VERY lean actually...\n";
    }
    return "";
}

/*
 * Function name: id
 * Description:   Identifies death and his belongings.
 */
id(str)
{
    return str == "death" || str == "moot" || str == "scythe" || str == "robe";
}

/*
 * Function name: take_it
 * Description:   Try to take something from death.
 */
take_it(str)
{
    
    string name;
    int extra;
    
    name = capitalize(this_player()->query_real_name());
    extra = random(900) + 10;
    
    if (str == "scythe" || str == "robe")
    {
	write(
"You take a firm grip on the " + str + " and try to pull it towards you.\n" +
"Death frowns, raps you smartly across your fingers with a bony hand and says:\n" +
"STUPID MORTAL. YOU JUST EARNED " + extra + " EXTRA YEARS IN PURGATORY!\n");

	return 1;
    }

}



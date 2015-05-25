/*
   An example shop coded by Nick, some Ideas and part of code taken from
   Tricky's shop in hobbitown.

*/

inherit "/std/room";
inherit "/lib/trade";

#include <stdproperties.h>
#include <macros.h>
#include <language.h>
#include <cmdparse.h>
#include <composite.h>
#include <filter_funs.h>

#define STORE_ROOM "/d/Genesis/doc/examples/trade/store"
	/* Where the items are stored */

#define MAXLIST 30
#define NF(xxx) notify_fail(xxx)
#define TP this_player()
#define TO this_object()
#define BS(xxx) break_string(xxx,75)
#define NUM 4

/*
 * Prototypes
 */
void config_trade_data();
object *sell_it(object *ob, string str, int check);
object *buy_it(object *ob, string str2, string str3);
int not_interesting(object obj);

/*
 * Function name:   create_room
 * Description:     Initialize this room
 * Returns:
 */
void
create_room()
{
    config_default_trade();
    set_money_give_max(1000);
    set_short("The local shop");
    set_long(
      "You are in the local shop. Here you can sell and buy stuff.\n" +
      "You can also use value or list commands, 'list armours',\n" +
      "'list weapons', 'list <name>' and 'list' works too.\n" +
      "There is also a small sign to read with more instructions.\n" +
      "");

    add_exit("/d/Genesis/doc/examples/trade/pub", "south", 0);
    add_exit(STORE_ROOM, "north", "@@wiz_check");

    add_prop(ROOM_I_INSIDE,1);  /* This is a real room */
}

/*
 * Function name:   init
 * Description:     Is called for each living that enters this room
 * Returns:
 */
void
init()
{
    ::init();   /* You MUST do this in a room-init */
    add_action("do_buy", "buy");
    add_action("do_sell", "sell");
    add_action("do_value", "value");
    add_action("do_list", "list");
    add_action("do_read", "read");
}

/*
 * Function name:   wiz_check
 * Description:     Check if a player is a wizard
 * Returns:         0 if the player is a wizard
 *                  1 otherwise
 */
int
wiz_check()
{
    if (this_player()->query_wiz_level())
      	return 0;
    write("Some magic force keeps you from going north.\n");
    say(QCTNAME(this_player()) + " tries to go north but fails.\n");
    return 1;
}

/*
 * Function name:   do_read
 * Description:     If a player wants to know what instuctions can be
 *                  used
 * Arguments:       str - string, hopefully "sign"
 * Returns:         1/0
 */
int
do_read(string str)
{
    NF("Read what?\n");
    if (str != "sign")
	return 0;

    write("" +
	"You can try these instructions: \n" +
	"    buy sword for gold and get copper back\n" +
	"    buy sword for gold coins\n" +
	"    sell sword for copper coins\n" +
	"    sell all  - will let you sell all items except for\n" +
        "                items you wield or wear.\n" +
	"    sell all! - will let you sell ALL items you have, well\n" +
	"                at least the droppable, and no coins.\n" +
	"    sell sword, sell second sword, sell sword 2, sell two swords\n" +
	"                also works. You might want to change the verb to\n" +
	"                'value' or 'buy' too. Bevare, you will never buy\n"+
	"                more than one item at a time. So if you really\n" +
	"                those three expensive swords, you have to repeat\n" +
	"                yourself three times.\n" +
	"    If you want a list of all swords available in the store, the\n" +
	"    correct syntax is: list sword\n" +
	"    (not: list swords, though this might change) \n" +
	"");
    return 1;
}

/*
 * Function name:   do_sell
 * Description:     Try to let the player sell the_item
 *                  Observe there is no message written when sold item
 *                  has a value higher than the shop gives out.
 * Returns:         1 on sucess
 * Arguments:       str - string holding name of item, hopefully
 */
int
do_sell(string str)
{
    object *item;
    int value, check;
    string str1, str2;

    if(!str || str =="")
    {
	NF("Sell what?\n");
	return 0;
    }

/*  Did player specify how to get the money? */
    if (sscanf(str, "%s for %s", str1, str2) != 2)
    {
 	str1 = str;
	str2 = "";
    }

    check = 1; /* Nothing worn or wielded will be sold. */

    if (str1 == "all!")
    {
	str1 = "all";
        check = 0; /* Unless the player wants it. */
    }

    if (parse_command(str1, TP, "%i", item))
	item = NORMAL_ACCESS(item, 0, 0);
    else
	item = ({ });

    NF("I can't find " + str1 + ".\n");
    if (sizeof(item) == 0)
	if (!(item = ({ present(str1, TP) }) ))
	    return 0;

    item = sell_it(item - ({ 0 }), str2, check);
    if (sizeof(item))
    {
	str2 = COMPOSITE_DEAD(item);
	say(QCTNAME(TP) + " sold " + str2 + ".\n");
	write("You sold " + str2 + ".\n");
	return 1;
    }

    if (str1 == "all")
	NF("You didn't sell anything.\n");

    return 0; /* Player tried to sell a non sellable object. */
}

/*
 * Function name:   sell_it
 * Description:     Try to let the player sell the item array
 * Returns:         1 on sucess
 * Arguments:       ob - the object array
 *                  check - wheather check for worn or wielded stuff
 *                  str - string describing how the money should be paid
 */
object *
sell_it(object *ob, string str, int check)
{
    int price, i, j, k, *tmp_arr, *null, *value_arr;
    object *sold;

    STORE_ROOM->short();

    value_arr = allocate(NUM);
    sold = allocate(sizeof(ob));

    for (i = 0; i < sizeof(ob); i++)
    {
        if (ob[i]->query_prop(OBJ_I_VALUE) == 0)
	{
	    /* Can not use query_short() below as that will not
	       resove VBFC. Which is right as query_short() should
	       return EXACTLY what was set by set_short() /Cmd 920727
            */
            NF("The " + ob[i]->short() + " has no value.\n");
	    continue;
        }

	if (check && (ob[i]->query_worn() ||
		      ob[i]->query_wielded()))
	{
	    NF("I normally don't buy wielded or worn objects.\n");
	    continue;
        }

/*  This will make all coins undroppable for a second. */
/*  Even fake coins, but then again, they have no value ;-) */
        ob[i]->query_auto_load();

        if (ob[i]->move(STORE_ROOM))
    	{
	    NF("It seems you are stuck with that.\n");
	    continue;
    	}

        if ((price = ob[i]->query_prop(OBJ_I_VALUE)) > 0)
	{

	    price = price * 100 / query_money_greed_sell();
	    /* This is not always necessary but here is the place to put it. */

            tmp_arr = calc_change(price, null, str);
            for (k = 0; k < NUM; k++)
                value_arr[k] += tmp_arr[k];

	    sold[j] = ob[i];
            j++;
	    if (j >= 20)
        	break;
/* Only let people sell 20 objects at once and hopefully we wont get those */
/* too long evaluation problems. */
	}
    }

    sold = sold - ({ 0 });

    if (sizeof(sold) > 0)
    {
        change_money(({ 0, 0, 0, 0 }) + value_arr, TP);
        write("You got " + text(value_arr) + ".\n");
    }

    return sold;
}

/*
 * Function name:   do_buy
 * Description:     Try to let the player buy an item
 * Returns:         1 on sucess
 * Arguments:       string - describing how to pay and get change
 */
int
do_buy(string str)
{
    object *item, store;
    string str1, str2, str3;

    if(!str || str =="")
    {
	NF("Buy what?\n");
	return 0;
    }

    STORE_ROOM->short();

    /*  Did the player specify payment and change? */
    if (sscanf(str, "%s for %s and get %s", str1, str2, str3) != 3)
    {
    /* Well, maybe player has defined how payment will be done atleast? */
        str3 = "";
        if (sscanf(str, "%s for %s", str1, str2) != 2)
        {
            str2 = "";
            str1 = str;
        }
    }

    store = find_object(STORE_ROOM);
    if (parse_command(str1, store, "%i", item))
    /* NORMAL_ACCESS wants the items to be in or next to the player, we will
       fool the NA with fool_filter. We want the other features NA provides
       you see. */
	item = NORMAL_ACCESS(item, "fool_filter", TO);
    else
	item = ({ });

    NF("No " + str1 + " present in the store room.\n");
    if (sizeof(item) == 0)
	if (!(item = ({ present(str1, store) }) ))
	    return 0;

    item = (item - ({ 0 })) - ({ store }); /* Don't buy the store room ! ;-) */

    item = buy_it(item, str2, str3);
    if (sizeof(item))
    {
	str2 = COMPOSITE_DEAD(item);
	say(QCTNAME(TP) + " bought " + str2 + ".\n");
	write("You bought " + str2 + ".\n");
	return 1;
    }

    return 0; /* Player tried to sell a non sellable object. */
}

/*
 * Function name:   buy_it
 * Description:     Try to let the player buy the item array
 * Returns:         1 on sucess
 * Arguments:       ob - the object array
 *                  str2 - string describing how the money should be paid
 *                  str3 - what coin types to get chainge back in
 */
object *
buy_it(object *ob, string str2, string str3)
{
    int price, i, j, k, *value_arr, *arr, error;
    object *bought;

    value_arr = allocate(2 * NUM);
    bought = allocate(sizeof(ob));

    for (i = 0; i < sizeof(ob); i++)
    {
	price = 2 * ob[i]->query_prop(OBJ_I_VALUE) *
	    query_money_greed_buy() / 100;
	/* If you don't feel greedy you can shorten the calculation above. */

	if (ob[i]->move(TP))
	{
	    NF("You cannot carry the item.\n");
	    continue;
	}

	if (sizeof(arr = pay(price, TP, str2, 0, 0, str3)) == 1)
	{
	    ob[i]->move(STORE_ROOM, 1);
            continue;  /* pay() handles notify_fail() call */
	}

	/* Detect if there was a move error. */
	if (error = arr[sizeof(arr) - 1])
	{
	    if (error < -1)
	    {
	    /* Couldn't take the money from player, the coins were stuck */
	       write("You are carrying magic money my frind, no deal!.\n");
		continue;
	    }
	    /* We don't want the money so no move error to us, if there was one
	       it was because the player couldn't hold all coins, and if so the
	       drop text is already written, but the deal is still on :) */
	}

        for (k = 0; k < 2 * NUM; k++)
            value_arr[k] += arr[k];

	bought[j] = ob[i];
        j++;
	if (j >= 1)
       	    break;
	/* Well, we don't want to let a player accidentily buy too much :) */
    }

    bought = bought - ({ 0 });

    if (sizeof(bought) > 0)
    {
        write("You pay " + text(arr[0 .. NUM - 1]) + ".\n");
	str2 = text(arr[NUM .. 2 * NUM - 1]);
	if (str2)
            write("Your change is " + str2 + ".\n");
    }

    return bought;
}


/*
 * Function name:   do_value
 * Description:     Let the player value an item, carry or in shop
 * Returns:         1 on success
 */
int
do_value(string str)
{
    object *item, store;
    int *arr, price, i, j;

    if(!str || str =="")
    {
	NF("Value what?");
	return 0;
    }

    if (parse_command(str, TP, "%i", item))
	item = NORMAL_ACCESS(item, 0, 0);
    else
	item = ({ });

    if (sizeof(item) == 0)
	item = ({ present(str, TP) });

    item = item - ({ 0 });

    for (i = 0; i < sizeof(item); i++)
    {
	if (not_interesting(item[i]))
	{
	    NF("That item is not interesting.\n");
	    continue;
	}

	price = item[i]->query_prop(OBJ_I_VALUE) *
	    query_money_greed_sell() / 100;
	arr = give(price, TP, "", 1);
	arr = exclude_array(arr, 0, NUM - 1);
	if (!text(arr))
	    write("That item is not worth anything here.\n");
	else
            write(BS("You would get " + text(arr) + " for the " +
                item[i]->short() + ".\n"));
	j++;
    }

    store = find_object(STORE_ROOM);

    if (parse_command(str, store, "%i", item))
	item = NORMAL_ACCESS(item, "fool_filter", TO);
    else
	item = ({ });

    NF("No " + str + " present in the store room.\n");
    if (sizeof(item) == 0)
	item = ({ present(str, store) });

    item = (item - ({ 0 })) - ({ store });

    for (i = 0; i < sizeof(item); i++)
    {
	price = 2 * item[i]->query_prop(OBJ_I_VALUE) *
	    query_money_greed_buy() / 100;

	arr = split_values(price); /* A price with few coins possible */
	if (!text(arr))
	    write("That item wouldn't cost you much.\n");
	else
            write(BS("I want " + text(arr) + " for the " +
		item[i]->short() + ".\n"));
	j++;
    }

    say(QCTNAME(TP) + " asks about some values.\n");
    if (j > 0)
	return 1;
    return 0;
}

/*
 * Function name:   do_list
 * Description:     Provide a list of objects in the store room
 * Returns:         0 if not recognised
 *                  1 otherwise
 * Arguments: 	    str - the name of the objects to search for
 */
int
do_list(string str)
{
    object *item_arr;
    int i, price, *arr;

    STORE_ROOM->short();

    item_arr = all_inventory(find_object(STORE_ROOM));

    if (!sizeof(item_arr))
    {
	NF("The store room is currently empty.\n");
	return 0;
    }

    if (str == "weapons")
        item_arr = filter(item_arr,"weapon_filter", TO);
    else
	if (str == "armours")
  	    item_arr = filter(item_arr,"armour_filter", TO);
    else
	if (str)
  	    item_arr = filter(item_arr,"string_filter", TO, str);

    if (sizeof(item_arr) < 1)
    {
	NF("No match found.\n");
	return 0;
    }

    for (i = 0; i < sizeof(item_arr); i++)
	if (i < MAXLIST)
	{
	    price = 2 * item_arr[i]->query_prop(OBJ_I_VALUE) *
		 query_money_greed_buy() / 100;

	    arr = split_values(price);
	    str = sprintf("%-25s",
		 capitalize(LANG_ADDART(item_arr[i]->short())));

	    if (!text(arr))
	        write(str + "That item wouldn't cost you much.\n");
	    else
	        write(BS(str + text(arr) + ".\n"));
	}
	else
	{
	    write("Truncated: " + i + ".\n");
	    break;
	}

    return 1;
}

/* Function to identify a weapon */
mixed
weapon_filter(object ob)
{
    return (function_exists("create_object", ob) == "/std/weapon");
}

/* Function to identify an armour */
mixed
armour_filter(object ob)
{
    return (function_exists("create_object", ob) == "/std/armour");
}

/* Function to identify the string in list command */
mixed
string_filter(object ob, string str)
{
    return ob->id(str);
}

/* Not interesting, 0 value or coin. */
int
not_interesting(object ob)
{
    if (!ob->query_prop(OBJ_I_VALUE) ||
	(function_exists("create_heap", ob) == "/std/coins"))
	return 1;
    return 0;
}

/* To fool the NORMAL_ACCESS() */
int
fool_filter(object ob) { return 1; }


/* This is an object that allows people to choose attributes from a
 * structured list contained in the PATH + check_attrib object.
 */

#define PATH "/d/Standard/login/"

inherit "/std/object";
#include "/sys/stdproperties.h"

int aval, bval, gval;

create_object()
{
    set_name("selector");
    set_short("attribute selector");
    set_long
	(
	 break_string
	 ("This machine has three dials labled alpha, beta and gamma. " +
	  "There is also a green button. Above there is a sign saying: " +
	  "Set the dials to preferred radiation. Then press the button.", 78) + "\n" +
	  "The dials can be set to 0 through 5. Example 'beta 3'.\n");
    add_prop(OBJ_I_NO_GET);
    set_no_show(1);
}

init()
{
    string *adj;
    
    add_action("press", "press");
    add_action("alpha", "alpha");
    add_action("beta", "beta");
    add_action("gamma", "gamma");
    add_action("show", "show");
    adj = this_player()->query_adj(1);
    this_player() -> remove_adj(adj);
    if (!adj || sizeof(adj) < 2)
	adj = ({ "", "" });
    if (!adj[0] || adj[0] ==  "")
	adj[0] = "plain";
    if (!adj[1] || adj[1] == "")
	adj[1] = "";
    this_player() -> set_adj(adj);
}

int
select(string str)
{
    string *adj;

    if(call_other(PATH + "check_attrib", "select", str)) {
	adj = (string *) this_player()->query_adj(1);
	this_player()->remove_adj(adj);
	adj = adj + ({"", ""});
	adj = slice_array(adj, 0, 1);
	adj[1] = adj[0];
	adj[0] = str;
	if (adj[1] == "")
	    adj = slice_array(adj, 0, 0);
	this_player() -> set_adj(adj);
    }
    else
	write("Unknown attribute.\n");
    return 1;
}

public int 
show(string str)
{
    write("This is how you look:\n");
    write(this_interactive()->long() + "\n");
    return 1;
}

int alpha(string str) 
{
    int val;
    if(!stringp(str)) {
	write("You have to set the dial to a value.\n");
	return 1;
    }
    if(sscanf(str, "%d", val) != 1) {
	write("You have to set the dial to a value.\n");
	return 1;
    }
    if(val < 0 || val > 5) {
	write("You can only set the dial to 0 through 5.\n");
	return 1;
    }
    write("You set the alpha dial to " + val + ".\n");
    aval = val;
    return 1;
}
    
int beta(string str) 
{
    int val;
    if(!stringp(str)) {
	write("You have to set the dial to a value.\n");
	return 1;
    }
    if(sscanf(str, "%d", val) != 1) {
	write("You have to set the dial to a value.\n");
	return 1;
    }
    if(val < 0 || val > 5) {
	write("You can only set the dial to 0 through 5.\n");
	return 1;
    }
    write("You set the beta dial to " + val + ".\n");
    bval = val;
    return 1;
}
    
int gamma(string str) 
{
    int val;
    if(!stringp(str)) {
	write("You have to set the dial to a value.\n");
	return 1;
    }
    if(sscanf(str, "%d", val) != 1) {
	write("You have to set the dial to a value.\n");
	return 1;
    }
    if(val < 0 || val > 5) {
	write("You can only set the dial to 0 through 5.\n");
	return 1;
    }
    write("You set the gamma dial to " + val + ".\n");
    gval = val;
    return 1;
}
    
int press(string str)
{
    
    select(call_other(PATH + "check_attrib", "get_elem", aval, gval));
    select(call_other(PATH + "check_attrib", "get_elem", gval, bval));
    write("As radiation flows through your body, you feel it change.\n");
	
    return 1;
}

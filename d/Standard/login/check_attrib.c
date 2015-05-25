/* An object for setting player attributes. Each player has 2 attributes
 * that are settable from a number of categories. A player may only have
 * one attribute from each category.
 * /Tintin 911002
 */
#define PATH "/d/Standard/login/"
inherit "/std/object";

/* Attrib contains one element for each category of attributes. Each element
 * conatins an array with 3 parts, category name, category type and an array
 * of category members.
 */
mixed *attrib;


create_object()
{
    obj_names = ({"selector"});
    setuid();
    seteuid(getuid(this_object()));
    restore_object(PATH + "attributes");
}

int
list(string str)
{
    string *tmp;
    int i, j;

    if (!str)
    {
	for (i = 0; i < sizeof(attrib); i++)
	{
	    write("Category: " + attrib[i][0] + "\n");
	    write(break_string(implode(attrib[i][2], ", "), 70) + "\n");
	}
	return 1;
    }

    str = lower_case(str);
    tmp = explode(str, " ");
    if (tmp[0] == "categories")
    {
	for (j = 0; j < sizeof(attrib); j++)
	    write(attrib[j][0] + " ");
	write("\n");
	return 1;
    }

    for (i = 0; i < sizeof(tmp); i++)
    {
	for (j = 0; j < sizeof(attrib); j++ )
	{
	    if (tmp[i] == lower_case(attrib[j][0]))
	    {
                write("Category: " + attrib[j][0] + "\n");
	        write(break_string(implode(attrib[j][2], ", "), 70) + "\n");
	    }
	}
    }
    return 1;
}

int
select(string str)
{
    int i;

    if (!str)
    {
	write("You have to select an attribute.\n");
	return 1;
    }
    for (i = 0; i < sizeof(attrib); i++)
    {
	if (sizeof(filter_array(attrib[i][2], "is_equal", this_object(), str)))
	    return 1;
    }
    return 0;
}

int
is_equal(string a, string b)
{
    if (a == b)
	return 1;
    return 0;
}

int types()
{
    return sizeof(attrib);
}

int num_elements(int ind)
{
    return sizeof(attrib[ind][2]);
}

string get_elem(int ind1, int ind2)
{
    return attrib[ind1][2][ind2];
}

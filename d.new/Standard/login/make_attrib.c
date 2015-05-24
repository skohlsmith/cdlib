
inherit "/std/object";

#define PATH "/d/Standard/login/"

mixed attrib;

void
create_object()
{
    string s;
    string * a;
    int i, j, k;

    i = 0;
    j = 0;
    attrib = ({ });

    seteuid(getuid(this_object()));
    s = read_file(PATH + "attributes");
    a = explode(s, "\n");

    while (a[0] == "")
	a = slice_array(a, 1, sizeof(a) - 1);
    while (a[(sizeof(a) - 1)] == "")
	a = slice_array(a, 0, sizeof(a) - 2);

    k = sizeof(a);
    
    while (i < k)
    {
	attrib = attrib + ({ ({ a[i++], 3, ({ }) }) });
	write(i + "\n");
	while (i < k && a[i] != "")
	    attrib[j][2] = attrib[j][2] + ({ a[i++] });
	i++;
	j++;
    }
    save_object(PATH + "attributes");
    call_out("remove_object", 2);
}


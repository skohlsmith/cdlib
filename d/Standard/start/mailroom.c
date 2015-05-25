#pragma strict_types
#pragma no_clone

inherit "/std/room";

#include <stdproperties.h>

#define MAILREADER	"/secure/mail_reader"
#define MAILCHECKER	"/secure/mail_checker"

void
create_room()
{
 
    set_short("The main post office");
    add_prop(ROOM_I_INSIDE,1); // this room is always lit

    set_long("You are at the main branch of the post office.\n"+
        "From here you can reach people with a written message, all over the universe!\n"+
        "Commands are:\n\n"+
        "    read            Read your mail.\n"+
        "    mail <name>     Mail to player 'name'.\n"+
        "    mail <name> -a  Mail anonymously to player 'name'. (Only for Wizards.)\n"+
        "    from            List all headers.\n\n");

    add_exit("/d/Standard/start/church","north","@@wblock");
}

int
wblock()
{
    return 0;
}

void
init()
{
    object reader;
    ::init();

    reader = clone_object(MAILREADER);
    if (!reader) {
	write("No mailreader is available at this time.\n");
	return;
    }
    reader->move(this_player());
}

void
leave_inv(object ob, mixed to)
{
    object reader;
    ::leave_inv(ob, to);

    reader = present("mailread", ob);
    if (reader)
	reader->remove_object();
}

int
query_mail(int silent)
{
    string new;
    int mail;

    mail = MAILCHECKER->query_mail(this_player());
    if (!mail)
	return 0;
    new = "";
    if (mail > 1)
    {
	new = " NEW";
    }
    write("There is" + new + " mail for you in post office.\n");
    return 1;
}

/*
  human_std.c

  This is the race object used for players of race: human
*/
inherit "/config/race/generic";

start_player()
{
    start_mail("/d/Standard/start/mailroom");
    ::start_player();
}

query_race() { return "human"; }


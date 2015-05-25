/* Domain defs */

#include "/sys/stdproperties.h"

#ifndef KRYNN_DEF
#define KRYNN_DEF

#define TP           this_player()
#define TO           this_object()
#define ADD(xxx,yyy) add_action(xxx,yyy);
#define ADA(xxx)     add_action(xxx,xxx);
#define NF(xxx)      notify_fail(xxx);
#define BS(xxx,nn)   break_string(xxx+"\n",nn)
#define C(xxx)       capitalize(xxx)
#define P(str, ob)   present(str, ob)
#define E(ob)        environment(ob)
#define L(str)       lower_case(str)

#define M_FILE     "/d/Krynn/open/monster/monster.c";
#define C_FILE     "/d/Krynn/open/monster/creature.c";
#define OPEN	   "/d/Krynn/open/"

#define STAND_DIR  "/d/Krynn/"
#define SOL_DIR    "/d/Krynn/solace"
#define RAST_DIR   "/d/Krynn/rastlin"
#define NICK_DIR   "/d/Krynn/nick"
#define ZAPP_DIR   "/d/Krynn/zapp"
#define SILVER_DIR "/d/Krynn/silverbane"
#define ASH_DIR    "/d/Krynn/ashbless"
#define EIRAM_DIR  "/d/Krynn/eiram"
#define DOR_DIR    "/d/Krynn/dorak"

#define QUESTH     "/d/Krynn/solace/village/monster/quest_m.h"

#define TELLDOMAIN(xxx) "/d/Krynn/private"->my_shout(xxx);
#define MESSAGE(xxx)    "/d/Krynn/private"->message(xxx);

#define GUILD          "/d/Krynn/solace/guild/guild_hall"
#define VOID           "/d/Krynn/common/void"

#define INSIDE  add_prop(ROOM_I_INSIDE, 1);
#define OUTSIDE add_prop(ROOM_I_INSIDE, 0);
#define DARK    add_prop(ROOM_I_LIGHT,0);
#define LIGHT   add_prop(ROOM_I_LIGHT,1);

#define NEVERKNOWN add_prop(LIVE_I_NEVERKNOWN, 1)
#define ALWAYSKNOWN add_prop(LIVE_I_ALWAYSKNOWN, 1)

#endif

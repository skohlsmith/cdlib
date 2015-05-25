/* Guild.h */

#include <formulas.h>
/* #include <local.h> */
#define TDIR 		"/d/Genesis/doc/examples/solamn/"
#define OBJ		"/d/Genesis/doc/examples/solamn/"

#define GUILD_NAME   	"Solamnian Knights"
#define BOOKPATH     	TDIR + "book/"
#define HELP_DIR     	TDIR + "log/help_dir/"
#define MEDALPATH    	OBJ + "medal"
#define O_SHADOW     	OBJ + "occ_shadow"
#define L_SHADOW     	OBJ + "lay_shadow"
#define KNIGHT_SOUL  	OBJ + "solamnian_soul"
#define POBJ		"/d/Krynn/solace/guild/obj/p_obj"
#define SPELL_OBJ	"/d/Krynn/solace/guild/spells/guild_spells"

#define JOINPATH     	TDIR + "log/joined"
#define GUIDANCEPATH 	TDIR + "log/guidance"
#define BANISHPATH   	TDIR + "log/banished"
#define PRESTIGEPATH	TDIR + "log/prestige"
#define LEAVEPATH    	TDIR + "log/left"
#define BANISH_FILE  	TDIR + "log/ban"
#define PRESTIGE_FILE	TDIR + "log/pre"
#define KILL_LOG     	TDIR + "log/kills"

#define ADMIN        	"/d/Krynn/solace/guild/admin"
#define ENTRANCE	TDIR + "entrance"
#define START_ROOM      TDIR + "starting"


#define SS_DBROSE	15
#define SS_DSQUIRE	30
#define SS_DCROWN	45
#define SS_DSHIELD	65
#define SS_DROSE	85
#define SS_USQUIRE	25
#define SS_UCROWN	35
#define SS_USHIELD	55
#define SS_UROSE	75
#define SS_USROSE	90

#define L_BROSE		1
#define L_SQUIRE	2
#define L_CROWN		3
#define L_SHIELD	4
#define L_ROSE		5
#define L_SROSE		6

#define TAX		({ 0, 6,  8, 12, 14,  17,  20 })
#define MAX_TRAIN	({ 0, 0, 40, 60, 80, 100, 100 })
#define KNIGHT_TITLES	({ "the Solamnian Knight", \
			"Knight of the Black Rose", \
			"Squire of Solamnia", \
			"Knight of the Crown", \
			"Knight of the Sword", \
			"Knight of the Rose" })

#define SS_RESCUE 	67530
#define SS_BASH   	67531
#define SS_KICK   	67532
#define KNIGHT_SKILL_DESC ({"rescue", "bash", "kick"})
#define SKILL_OB	"/lib/skill_raise"

#define SS_PRESTIGE_ACC	108111  /* Krynn is domain#8 */
#define SS_PRESTIGE_ST	108112  /* Krynn is domain#8 */

#define GUILD_GROUP 1
#define MEMBER_BIT 0
#define BANISH_BIT 1
#define LAYMAN_BIT 2

#define MEMBER(ob)     ob->test_bit("Krynn", GUILD_GROUP, MEMBER_BIT)
#define LAYMAN(ob)     ob->test_bit("Krynn", GUILD_GROUP, LAYMAN_BIT)
#define BANISHED(ob)   ob->test_bit("Krynn", GUILD_GROUP, BANISH_BIT)

#define IN_B_LIST(str, remove)	ADMIN->in_banish_list(str, remove)
#define REM_B_LIST(str)    	ADMIN->remove_from_list(str)
#define IN_P_LIST(str, remove)	POBJ->in_prestige_list(str, remove)
#define BANISH(ob, reason)	ADMIN->banish_now(ob, reason)
#define KILLED(killer, killed)	ADMIN->killed_logging(killer, killed)
#define LEAVE(ob)		ADMIN->leave(ob)

              /*  2H  UNA PAR DEF SW KN CL */
#define SKILLS ({ 20, 21, 23, 24,  0, 3, 4, SS_RESCUE, SS_BASH, \
	SS_KICK })

/* Formulas to the cmd_soul */
#define F_CKICK_HIT(ob, ob2)	ob->query_skill(SS_KICK) * \
					ob->query_stat(SS_DEX) 

#define F_CKICK_PEN(ob, ob2)	F_PENMOD(ob->query_stat(SS_STR) / 4, \
					ob->query_skill(SS_KICK) / 2) + 10

#define CKICK_HIT		1000

#define F_BASH_HIT(ob, ob2)	ob->query_stat(SS_DIS) * \
					ob->query_skill(SS_BASH)

#define F_BASH_PEN(ob, ob2)	F_PENMOD(ob->query_stat(SS_STR) / 2, \
					2 * ob->query_skill(SS_BASH) / 3) + 20

#define BASH_HIT		1500

#define F_RESCUE(ob, ob2)	ob->query_stat(SS_DIS) * \
					ob->query_stat(SS_DEX) *  \
					ob->query_skill(SS_RESCUE)

#define RESCUE_HIT		8000


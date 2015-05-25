/*
  /config.std/sys/config.h

  This file contains all world specific information. To configure your
  own world and set up your own mud, make changes to this file.


*/
#ifndef CONFIG_DEFINED
#define CONFIG_DEFINED

/*
 * Use this if you want to convert players logging in from 2.4.6 save files
 */
#ifndef CONVERT_PLAYERS   /* Enables conversion of 2.4.6 save files. */
#define CONVERT_PLAYERS
#endif CONVERT_PLAYERS

/*
 *
 * MUD_NAME
 *
 * The name of the mud. This will be defined in all files compiled.
 * It is either a single string or a mapping of one name for each
 * port it might be running on. '0' in the mapping means default name.
 *
 */
#undef MUD_NAME ([ 0: "Unconfigured_test", 3000: "Unconfigured" ])

/*
 * DEFAULT_WIZARDS
 *
 * This array contains the default setup of wizards of this LPmud.
 */
/*
#define DEFAULT_WIZARDS (["root": ({50, "root", "Standard", "root",}), \
			  "wiz" : ({35, "root", "Wiz", "root",}),])
*/
#define DEFAULT_WIZARDS (["root": ({WIZ_KEEPER, 50, "root", "Standard", "root", 0, "", ({}) }), \
                          "wiz" : ({WIZ_LORD,   35, "root", "Wiz",      "root", 0, "", ({}) }), ])

/*
 * Guesses
 */
#define BASE_DOMAIN "Standard"
#define LOGIN_TEST_PLAYER "root"

/*
 * Just a guess.  Part of the ateam code.  The aop team has extra read privs
 * (see master.c)  Seems to be a Genesis thing.
 */
#define AOP_TEAM_LOGS ({ "aop_team", })

/*
 * DEFAULT_DOMAINS
 *
 * This array contains the default setup of domains of this LPmud.
 */
/*#define DEFAULT_DOMAINS (["Standard":	({ 0, "root", ({"root",}),1, 0, 0, "", ([]), ([]),}), \
			  "Wiz": 	({ 1, "wiz", ({"wiz",}), 1, 0, 0, "", ([]), ([]),}),]) */

#define DEFAULT_DOMAINS (["Standard":	({ 0, "std", "root", "", ({"root",}), 1, 0, 0, 0, }), \
			  "Wiz": 	({ 1, "wiz", "wiz",  "", ({"wiz",}) , 1, 0, 0, 0, }),])

/*
 * WIZARD_DOMAIN
 *
 * This is the name of the special domain where all 'lonely' wizards are
 * placed. This domain has no domainlord and no common, open dirs
 *
 */
#define WIZARD_DOMAIN "Wiz"

/*
 * OPEN_LOG_DIR
 *
 * This is the location of the open logs.
 */
#define OPEN_LOG_DIR "/d/Standard/log"

/*
 * DISCARD_DOMAIN_DIR
 *
 * This is the name of the directory where all the removed domains are
 * put awaiting further developments.
 */
#define DISCARD_DOMAIN_DIR "/d/Discarded"

/*
 * DEFAULT_PLAYER
 *
 * This is the default player file if nothing else is defined
 */
#define DEFAULT_PLAYER "/d/Standard/race/human_std"

/*
 * DEFAULT_START
 *
 * This is the default starting location if nothing else is defined
 */
#define DEFAULT_START "/d/Standard/start/church"

/*
 * STARTING PLACES
 *
 * This array contains all the places that are acknowledged starting places
 * by default.
 *
 */
#define STARTING_PLACES ({ "/d/Standard/start/church",})
#define TEMP_STARTING_PLACES ({ })

/*
 * WIZ_ROOM
 *
 * This is the path to the cozy room to which we set new wizards starting
 * location.
 */
#define WIZ_ROOM "/d/Standard/start/church"

/*
 * ADMIN_HOME
 *
 * This is the administrators workroom. Has some special functions in the game.
 */
#define ADMIN_HOME "/d/Standard/start/church"

/*
 * WIZ_APPRENTICE_SCROLL
 *
 * This is the path to the scroll that gets loaded and given to new wizards.
 * K{rt barn har m}nga namn...
 */
#define WIZ_APPRENTICE_SCROLL "/d/Standard/obj/init_scroll"
#define APPRENTICE_SCROLL_FILE "/d/Standard/doc/appr_scroll_file"

/*
 * LOGIN_NEW_PLAYER
 *
 * This is the player object that manages all entering of new players
 */
#define LOGIN_NEW_PLAYER "/d/Standard/login/ghost_player"

/*
 * WIZ_MAKER
 *
 * This is a list of the paths to objects that may create apprentices
 */
#define WIZ_MAKER ({ "/d/Standard/start/church" })

/*
 * LOGIN_FILE_WELCOME
 * LOGIN_FILE_NEWS
 *
 * This is the names of the files that are written to user logging in.
 */
#define LOGIN_FILE_WELCOME "/d/Standard/doc/login/WELCOME"
#define LOGIN_FILE_NEWS "/d/Standard/doc/login/NEWS"

/*
 * MAIL_INFO_OBJ
 * SPECIAL_INFO_OBJ
 *
 * These are the objects that the finger_player object uses to find out
 * interesting stuff about the player
 */
#define MAIL_INFO_OBJ "/d/Standard/std/mail_stuff"
#define SPECIAL_INFO_OBJ "/d/Standard/std/special_stuff"

/*
 * DEFAULT_DEATH tells what object should be loaded when a player dies
 */
#define DEFAULT_DEATH "/d/Standard/death/death_mark"

/*
 * DOMAIN_LINK
 * WIZARD_LINK
 *
 * Filenames trailing /d/Domain_name/ and /d/<WIZARD_DOMAIN>/wizard_name/ to
 * preload on startup and call at shutdown.
 */
#define DOMAIN_LINK "domain_link"
#define WIZARD_LINK "wizard_link"

/*
 * PRELOAD_DOMAIN
 * PRELOAD_WIZARD
 *
 * Filenames trailing /d/Domain_name/ and /w/wizard_name/ to preload on startup
 *
 */
#define PRELOAD_DOMAIN "preload"
#define PRELOAD_WIZARD "preload"

/*
 * GAMEINFO_LOGIN
 * GAMEINFO_INFO
 *
 * Information about the game in login procedure.
 */
#define GAMEINFO_LOGIN	"gameinfo"
#define GAMEINFO_INFO	"/d/Standard/doc/login/GAMEINFO_INFO"

/*
 * APPLICATION_LOGIN
 * APPLICATION_INFO
 * APPLICATION_BOARD_LOC
 *
 * Information about the game in login procedure.
 */
#define APPLICATION_LOGIN	"application"
#define APPLICATION_INFO	"/d/Standard/doc/login/APPLICATION_INFO"
#define APPLICATION_BOARD_LOC	"/d/Standard/application"

/*
 * SECURE_ARRAYS
 *
 * If this is true then memory is sacrificed to stop meddling with arrays
 * whose elements are meant to be read_only.
 */
#define SECURE_ARRAYS 1

/*
 * COMBAT_FILE
 *
 * This file is cloned to manage a fight for a living object.
 */
#define COMBAT_FILE "/std/combat/cbase"

/*
 * MAX_PLAY
 *
 * This is the maximum amount of players that may be logged on at
 * one time.
 */
#define MAX_PLAY 40

/*
 * ARCH_SLOTS
 *
 * This is the number of slots reserved for arches or keepers.
 */
#define ARCH_SLOTS 1

/*
 * MIN_SLOTS
 *
 * This is the minimum number of slots that may remain before
 * quotation of wizards/players is being considered.
 */
#define MIN_SLOTS 3

/*
 * WIZ_RATIO
 *
 * This is the ratio of Wizards in percent that is being
 * applied when quotation is computed.
 */
#define WIZ_RATIO 30

/*
 * SMALL_MEMORY_LIMIT
 * LARGE_MEMORY_LIMIT
 *
 * This is the maximum memory allowed by the master object before itself
 * starts Armageddon. The memory consumption is fetched from the
 * debug("malloc") efun as the number before (a). The use of SMALL/LARGE
 * is determined by external calls to the gamedriver.
 */
#undef SMALL_MEMORY_LIMIT 10000000
#undef LARGE_MEMORY_LIMIT 28000000

/*
 * SWAP_MEM_MIN
 * SWAP_MEM_MAX
 * SWAP_TIME_MIN
 * SWAP_TIME_MAX
 *
 * These are swapping control parameters and control when we do swapping.
 * The swapping will start when we have used SWAP_MEM_MIN bytes of memory,
 * when we reach SWAP_MEM_MAX we will swap as much as possible. We will
 * not swap objects unless it is SWAP_TIME_MIN seconds old and we will
 * swap all objects older than SWAP_TIME_MAX seconds.
 */
#define SWAP_MEM_MIN    0
#define SWAP_MEM_MAX    5777216
#define SWAP_TIME_MIN   60
#define SWAP_TIME_MAX   900

/*
 * ARMAGEDDON
 *
 * This is the object to call "slow_shut" in when the GD tells master
 * that the memory is getting low.
 */
#define ARMAGEDDON "/secure/armageddon"

/*
 * NPC_SOULS
 *
 * The array of sould that all mobiles should have
 */
#define NPC_SOULS ({ "/cmd/std/soul_cmd", "/cmd/std/misc_cmd" })

/*
 * MAX_IDLE_TIME
 *
 * The maximum time you are guaranteed to be able to stay idle before
 * being logged out. The amount is counted in seconds.
 */
#define MAX_IDLE_TIME 1800

/*
 * MET_ACTIVE
 *
 * If MET_ACTIVE is defined the met system is enabled. If it is undefined
 * players will know each other without being introduced.
 */
#define MET_ACTIVE

/*
 * MAX_COMB_EXP_RED
 *
 * The maximum your combat exp gained will be reduced due to low quest exp.
 * 750 = 75 % reduction. When a LPmud gets more quests this figure can be
 * raised to 1000, or if you want no reduction, set to 0.
 */
#define MAX_COMB_EXP_RED 700.0

/*
 * MAX_EXP_RED_FRIENDLY
 *
 * The maximum xp a mortal can have to be treated friendly in the xp
 * reduction formula, i.e. to not have it take full effect but only up to
 * the reduction factor defined above in MAX_COMB_EXP_RED.
 */
#define MAX_EXP_RED_FRIENDLY 12000000

/*
 * DAY_AND_NIGHT
 *
 * If DAY_AND_NIGHT is active there will be a day night cyclus from 10 pm
 * to 5 am.
 */
#define DAY_AND_NIGHT

/*
 * STATUE_WHEN_LINKDEAD
 *
 * Show players as statues when they are linkdead.
 *
 */
#define STATUE_WHEN_LINKDEAD
#define STATUE_DESC "statue of"
#define STATUE_TURNS_INTO "turns into stone"
#define STATUE_TURNS_ALIVE "turns alive again"

/*
 * RELAY_HOST    - List of hosts that relays to the mud
 * RELAY_QUERY
 * RELAY_FORCE   - If defined, forces all connections through the relay
 *		   except if from the hosts in the array
 *
 *
 * These are really Genesis specific and should remain undef'ed.
 */
#undef RELAY_HOST ({ "milou.cd.chalmers.se", "129.16.79.12" })
#undef RELAY_QUERY "@@@host@@@\n"
#undef RELAY_FORCE ({ "sponsz.cd.chalmers.se", "129.16.79.31", "castafiore.cd.chalmers.se" })

/*
 * UDP_MANAGER
 *
 * If defined, this object manages udp messages.
 */
#define UDP_MANAGER "/sys/global/udp"

/*
 * DECAY_XP
 *
 * The part of xp decayed, in the ranking list, each 15 minutes.
 * (xp -= (xp > DECAY_XP ? xp / DECAY_XP : 1)
 * If this is 0 then no decay occurs.
 */
#define DECAY_XP 250

/*
 * DOMAIN_RANKWEIGHT_FORMULA
 *
 * This is the formula for calculating the weight factor in the domain
 * ranking list. q and c are quest and combat xp given to mortals by the
 * domain. Normally it is good to give quest xp and bad to give combat xp.
 * This is so because we want to promote the making of quests.
 */
#define DOMAIN_RANKWEIGHT_FORMULA(q, c)  (100 + (q / 25) + ((-c) / 10000))

/*
 * REGULAR_REBOOT
 *
 * If defined, the hour during which the game will be rebooted.
 */
#undef REGULAR_REBOOT 4

/*
 * NO_ALIGN_TITLE
 *
 * Defined if alignment title is not used
 */
#undef NO_ALIGN_TITLE

/*
 * CYCLIC_LOG_SIZE
 *
 * This is a mapping containing the maximum file size for logs for a given
 * euid. Normally the maximum file size for a log is given with the euid '0',
 * however, if the name of the euid appears in this mapping, the owner of the
 * euid has the option of allowing the size grow to the given value before
 * cycling is done.
 * If the size is less than 0, the log will be allowed to grow indefinately.
 */
#define CYCLIC_LOG_SIZE ([ 0 : 1000, "root" : -1, "Standard" : 20000 ])

/*
 * PRELOAD_FIRST
 *
 * Information on what files are to be preloaded before anything else.
 * This can either be a string and is then a filename holding all
 * files to be preloaded.
 *
 * It can also be an array holding the files to preload.
 */
#define PRELOAD_FIRST "/secure/preload.data"

/*
 * LOGIN_NO_NEW
 *
 * Define this to disable logins of new players, except for wizard helpers.
 */
#undef LOGIN_NO_NEW "/d/Standard/login/no_new_players"

/*
 * LOCKOUT_START_TIME
 * LOCKOUT_ENT_TIME
 * LOCKOUT_LEVEL
 *
 * Do not allow players below LOCKOUT_LEVEL to access the game between the
 * hours of LOCKOUT_START_TIME and LOCKOU_END_TIME. These are all integers,
 * the TIME being the hour of the day. Wizards that are allowed to enter the
 * game can have their jr's entering too, if Nick is allowed to enter, then
 * Nickjr is too. (There is no test if Nick did create Nickjr though :-/ )
 * Setting LOCK_OUT level to 51 stops most people. These are used in login.c
 */
#undef LOCKOUT_START_TIME	8
#undef LOCKOUT_END_TIME		20
#undef LOCKOUT_LEVEL		1

#endif

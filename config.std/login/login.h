/*

  Some constants relevant to the login procedure

*/

/*
 * PATH
 *
 * The path in which the login rooms reside
 */
#define PATH "/d/Standard/login/"

/*
 * LOGIN_FILE_NEW_PLAYER_INFO
 *
 * This is the names of the files that are written to new users logging in.
 */
#define LOGIN_FILE_NEW_PLAYER_INFO "/d/Standard/doc/login/NEW_PLAYER_INFO"

/*
 * RACEMAP
 *
 * This mapping holds the names of the allowed player races in the game and
 * which player file to use for new characters of a given race.
 */
#define RACEMAP ([                                          \
		"human"  : "/d/Standard/race/human_std",   \
		"vogon"  : "/d/Standard/race/vogon_std"    \
		])
/*
 * RACEATTR
 *
 * This mapping holds the standard attributes for each race. The attributes
 * are:
 *      standard height         (cm)
 *      standard weight         (kg)
 *      standard opinion        (0-100)
 *      standard appereance     (0-100)
 *      standard volume         (dm^3)
 *      standard width          (cm)
 */
#define RACEATTR ([                                \
		"human"  : ({180, 70, 50, 50, 70, 100 }),   \
		"vogon"  : ({ 90, 50, 20, 20, 50, 110 }),   \
		])

/*
 * when m_indexex work on constants: m_indexes(RACEMAP)
 */
#define RACES ({ "human", "vogon" })

/*
 * RACES_SHORT
 *
 * This mapping contains a short name for each race, abbreviated to exactly
 * three letters.
 */
#define RACES_SHORT ([ \
        "human" : "hum", \
        "vogon" : "vog" ])

/*
 * RACESTART
 *
 * This mapping holds the files of the starting locations for each race.
 */
#define RACESTART ([                                                   \
		    "human"  : "/d/Standard/start/church",   \
		    "vogon"  : "/d/Standard/start/church",   \
	          ])

/*
 * RACEPOST
 *
 * This mapping holds the files of the post offices locations for each race.
 */
#define RACEPOST ([                                                   \
		    "human"  : "/d/Standard/start/mailroom",   \
		    "vogon"  : "/d/Standard/start/mailroom",   \
	          ])

/*
 * RACESTAT
 *
 * This mapping holds the stats that each race has on start
 */
#define RACESTAT ([            /* str, dex, con, int, wis, dis */      \
		    "human"  : ({   9,  9,   9,   9,   9,   9 }),      \
		    "vogon"  : ({   4, 17,   8,  11,   7,   7 }),      \
	          ])


/*
 * RACEMISCCMD
 *
 * This mapping holds the files of the souls that should be used as
 * misc command soul for each race
 */
#define RACEMISCCMD ([                                           \
		    "human"  : "/d/Standard/cmd/misc_cmd_human",  \
		    "vogon"  : "/d/Standard/cmd/misc_cmd_vogon",  \
	          ])

/*
 * RACESOULCMD
 *
 * This mapping holds the files of the souls that should be used as
 * misc command soul for each race
 */
#define RACESOULCMD ([						 \
		    "human"  : "/d/Standard/cmd/soul_cmd_human",  \
		    "vogon"  : "/d/Standard/cmd/soul_cmd_vogon",  \
	          ])
/*
 * RACESOUND
 *
 * What sound do mainindex-race hear when subindex race speaks
 */
#define RACESOUND ([						\
		    "human" : ([ 				\
				"human" : "says",		\
				"vogon"	: "grunts",		\
				]),				\
		    "vogon" : ([				\
				"human" : "yells",		\
				"vogon"	: "says",		\
				])                              \
		    ])


/*
 * HEIGHTDESC
 */
#define HEIGHTDESC ({"very short", "short", "normal", "tall", "very tall" })

/*
 * WIDTHDESC
 */
#define WIDTHDESC ({"skinny", "lean", "normal", "plump", "fat" })

/*
 * SPREAD_PROC
 */
#define SPREAD_PROC ({ 60, 80, 100, 120, 140 })

#define LOGIN_NO_NEW "/d/Standard/login/no_new_players"
#define CONVERT_OLD "/d/Standard/login/convert_old_players"

/*
 * This is the current state of this ghost (uses set_ghost() / query_ghost())
 */

#define GP_BODY		1
#define GP_MANGLE	2
#define GP_FEATURES	4
#define GP_SKILLS       8
#define GP_NEW          (GP_BODY | GP_MANGLE | GP_FEATURES | GP_SKILLS)
#define GP_CONVERT      16
#define GP_DEAD         32

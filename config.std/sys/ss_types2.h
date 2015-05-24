/*
 * ss_types2.h
 *
 * This file defines the available stats and skills that are locally
 * configured. The only use of this file is that it should be included
 * by /sys/ss_types.h.
 */

/* List of skills as is going to be used */

#define SS_WEP_FIRST		0   /* The first weapon index */

#define SS_WEP_ASSRIFLE		SS_WEP_FIRST + 0  /* W_ASSRIFLE */
#define SS_WEP_PSI		SS_WEP_FIRST + 1  /* W_PSI */
#define SS_PILOT                121               /* PILOT */

				     /*   Description     Costfactor(0-100) */
#define SS_SKILL_DESC ([ \
/* Weapon skills */                                                    \
		      SS_WEP_ASSRIFLE:	({ "assault rifle",	    70, SS_DEX, 125 }), \
		      SS_WEP_PSI:	({ "psionic blast",	    93, SS_INT, 125 }), \
/* General fighting skills */					       \
		      SS_2H_COMBAT: 	({ "two handed combat",	   100, SS_DEX, 110 }), \
		      SS_UNARM_COMBAT: 	({ "unarmed combat",	    90, SS_STR, 110 }), \
		      SS_BLIND_COMBAT: 	({ "blindfighting",	    95, SS_DEX, 110 }), \
		      SS_PARRY: 	({ "parry",		    80, SS_STR, 110 }), \
		      SS_DEFENCE: 	({ "defence",		    80, SS_DEX, 110 }), \
		      SS_MOUNTED_COMBAT:({ "mounted combat",        50, SS_STR, 150 }), \
/* Magic skills */ 						       \
		      SS_SPELLCRAFT:	({ "spellcraft", 	    70, SS_INT, 125 }), \
		      SS_HERBALISM:	({ "herbalism", 	    70, SS_WIS, 125 }), \
		      SS_ALCHEMY:	({ "alchemy", 		    70, SS_INT, 125 }), \
\
		      SS_FORM_TRANSMUTATION: ({ "transmutation spells", 90, SS_INT, 110 }), \
		      SS_FORM_ILLUSION:    ({ "illusion spells",    70, SS_INT, 110 }), \
		      SS_FORM_DIVINATION:  ({ "divination spells",  70, SS_INT, 110 }), \
		      SS_FORM_ENCHANTMENT: ({ "enchantment spells", 80, SS_INT, 110 }), \
		      SS_FORM_CONJURATION: ({ "conjuration spells", 80, SS_INT, 110 }), \
		      SS_FORM_ABJURATION:  ({ "abjuration spells",  70, SS_INT, 110 }), \
		      SS_ELEMENT_FIRE:     ({ "fire spells",        70, SS_WIS, 110 }), \
		      SS_ELEMENT_AIR:      ({ "air spells",         70, SS_WIS, 110 }), \
		      SS_ELEMENT_EARTH:    ({ "earth spells",       70, SS_WIS, 110 }), \
		      SS_ELEMENT_WATER:    ({ "water spells",       70, SS_WIS, 110 }), \
		      SS_ELEMENT_LIFE:     ({ "life spells",        80, SS_WIS, 110 }), \
		      SS_ELEMENT_DEATH:    ({ "death spells",       90, SS_WIS, 110 }), \
/* Thief skills */						       \
		      SS_OPEN_LOCK:	({ "open lock", 	    70, SS_DEX, 110 }), \
		      SS_PICK_POCKET:	({ "pick pocket", 	    70, SS_DEX, 110 }), \
		      SS_ACROBAT:	({ "acrobat", 		    70, SS_DEX, 110 }), \
		      SS_FR_TRAP:	({ "find and remove traps", 70, SS_DEX, 110 }), \
		      SS_SNEAK:		({ "sneak", 		    70, SS_DEX, 125 }), \
		      SS_HIDE:		({ "hide", 		    70, SS_DEX, 125 }), \
		      SS_BACKSTAB:	({ "backstab", 		    70, SS_DEX, 110 }), \
/* General skills */						       \
		      SS_APPR_MON:	({ "appraise enemy",	    50, SS_WIS, 150 }), \
		      SS_APPR_OBJ:	({ "appraise object",	    50, SS_WIS, 150 }), \
		      SS_APPR_VAL:	({ "appraise value", 	    50, SS_WIS, 150 }), \
		      SS_SWIM:		({ "swim", 		    50, SS_STR, 150 }), \
		      SS_CLIMB:		({ "climb", 		    50, SS_STR, 125 }), \
		      SS_ANI_HANDL:	({ "animal handling", 	    50, SS_WIS, 125 }), \
		      SS_LOC_SENSE:	({ "location sense", 	    50, SS_WIS, 110 }), \
		      SS_TRACKING:	({ "tracking", 		    50, SS_WIS, 125 }), \
		      SS_HUNTING:	({ "hunting", 		    50, SS_INT, 125 }), \
		      SS_LANGUAGE:	({ "language", 		    50, SS_INT, 110 }), \
		      SS_AWARENESS:	({ "awareness", 	    50, SS_WIS, 110 }), \
		      SS_TRADING:	({ "trading", 		    50, SS_INT, 110 }), \
		      SS_RIDING:        ({ "riding",                50, SS_DEX, 125 }), \
		      SS_PILOT: 	({ "pilot", 		    50, SS_INT, 125 }), \
			])


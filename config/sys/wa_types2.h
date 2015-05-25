/*
 * wa_types2.h
 *
 * This file defines the available weapons are locally configured.
 * The only use of this file is that it should be included
 * by /sys/wa_types.h.
 */

/* Weapon types */

#define W_FIRST     0		/* The first weapon index */

#define W_ASSRIFLE  W_FIRST + 0
#define W_PSI       W_FIRST + 1
#define W_MISSILE   W_FIRST + 2
#define W_KNIFE     W_FIRST + 3

#define W_NO_T      4		/* The number of weapons defined */

/*
 * Drawbacks are arrange for each weapon type ({ dull, corr, break })
 * and types are ({ sword, polearm, axe, knife, club, missile, javelin })
 * With dull means generally that the weapon gets in worse condition
 * since it has been used a lot. Clubs etc don't get dull ;-)
 */
#define W_DRAWBACKS ({ ({ 5, 5, 5 }), ({ 4, 4, 4 }) })


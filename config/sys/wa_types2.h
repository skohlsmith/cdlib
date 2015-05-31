/*
 * wa_types2.h
 *
 * This file defines the available weapons are locally configured.
 * The only use of this file is that it should be included
 * by /sys/wa_types.h.
 */

/* Weapon types */

/* The first weapon index */
#define W_FIRST     0

/* These are used in the old login and example/old_ex code */
#define W_ASSRIFLE  W_FIRST + 0
#define W_PSI       W_FIRST + 1
#define W_MISSILE   W_FIRST + 2

#define W_KNIFE     W_FIRST + 3

/* This is used by /std/bow.c -- an unstrung bow is used like a polearm */
#define W_POLEARM   W_FIRST + 4

/* The number of weapons defined */
#define W_NO_T      5

/*
 * Drawbacks are arrange for each weapon type ({ dull, corr, break })
 * and types are ({ sword, polearm, axe, knife, club, missile, javelin })
 * With dull means generally that the weapon gets in worse condition
 * since it has been used a lot. Clubs etc don't get dull ;-)
 */
#define W_DRAWBACKS ({ ({ 0, 5, 5 }), ({ 0, 0, 0 }), ({ 0, 0, 5 }), ({ 4, 4, 4 }) \
({ 0, 0, 5 }) })

#define W_NAMES	({ "assault rifle", "psi", "missile", "knife", "polearm" })

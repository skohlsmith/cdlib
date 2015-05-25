/*
 * formulas2.h
 *
 * This file defines some weapon dependant defines.
 * The only use of this file is that it should be included
 * by /sys/formulas.h.
 */

/*
   If you insert changes you are recommended to copy /sys/formulas.h
   to here and make changes. It is important that the below define is
   defined afterwards:

#ifndef F_FORMULAS
#define F_FORMULAS
#endif

*/

#define F_LEGAL_TYPE(type)   ((type) == W_ASSRIFLE || \
			      (type) == W_PSI)

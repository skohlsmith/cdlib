/*
 * money2.h
 *
 * This file defines the local currency units.
 * The only use of this file is that it should be included by /sys/money.h.
 */

#define SIZEOF_MONEY_TYPES 3
#define MONEY_TYPES  ({ "cent", "credit", "megabuck" })
#define MONEY_SHORT  ({ "c",    "C",      "M" })
#define MONEY_VALUES ({ 1       ,  100     , 100000000 })
#define MONEY_WEIGHT ({ 7      ,  42     , 10023 })
#define MONEY_VOLUME ({ 3       ,  2      , 913 })

#define MONEY_MAKE_CENT(num)     MONEY_MAKE(num, "cent")
#define MONEY_MAKE_CREDIT(num)   MONEY_MAKE(num, "credit")
#define MONEY_MAKE_MEGABUCK(num) MONEY_MAKE(num, "megabuck")

#define MONEY_MOVE_CENT(num, from, to)     MONEY_MOVE("cent",     num, from, to)
#define MONEY_MOVE_CREDIT(num, from, to)   MONEY_MOVE("credit",   num, from, to)
#define MONEY_MOVE_MEGABUCK(num, from, to) MONEY_MOVE("megabuck", num, from, to)

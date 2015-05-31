This is based on https://github.com/xxx/cdlib, but that repo was missing the
/config and /d directory trees, which are necessary to have a working MUD.

Those directories have been rebuilt based on a *really* old (~1995) backup
I had of the cdlib mudlib and a slightly-newer (but modified) working MUD
backup.

I've tried to minimize any changes to try and keep this as faithful to the
original as I can, limiting changes to things required to make it work,
particularly with the current CD driver (06.04), from
https://github.com/cotillion/cd-gamedriver

Installation

- Have a base directory for the MUD, we'll use ~/mud
- compile and install the gamedriver to ~/mud/bin
- clone this cdlib repository to ~/mud/lib
- cd ~mud/lib
- run ./preparedirs.sh in order to create required empty subdirectories
- mv config.std to config
- mv d.new to d
- Start the driver
- The default Keeper-level player is named root and whose password is root
  You'll probably want to change that.

Any changes to make how your MUD is unique (weapon types, money types,
races, etc) should all be in files under ~mud/lib/config/

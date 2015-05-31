#!/bin/sh
#
# This creates the directories which the MUD expects to be there,
# but which are empty (and thus not tracked by git)
#
# RUN THIS FROM YOUR BASE lib DIRECTORY (i.e. where this script is)
#
mkdir -p d/Standard/ateam
mkdir -p d/Standard/log
mkdir -p d/Standard/open
mkdir -p d/Standard/domain/private
mkdir -p syslog/backup
mkdir -p syslog/log/enter
mkdir -p players/messages
mkdir -p players/sanctions/Standard
mkdir -p players/mail
mkdir -p d.new/Standard/ateam
mkdir -p d.new/Standard/log
mkdir -p d.new/Standard/open
mkdir -p d.new/Standard/domain/private
mkdir -p w
mkdir -p log

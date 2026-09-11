#!/usr/bin/env bash

#   Copyright © 2019 by The xTox Project Contributors
#
#   This file is part of xTox, a Qt-based graphical interface for Tox.
#   xTox is libre software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   xTox is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with xTox.  If not, see <http://www.gnu.org/licenses/>


# A xTox profile migrater for OSX
now=$(date +"%m_%d_%Y-%H.%M.%S")
bak="~/.Tox-Backup-$now"

echo "Figuring out if action is required ..."
if [ -d ~/Library/Preferences/tox ]; then
	echo "Moving profile(s) ..."
	cp -r ~/Library/Preferences/tox ~/Library/Application\ Support/
	mv ~/Library/Application\ Support/tox/ ~/Library/Application\ Support/Tox
	mv ~/Library/Preferences/tox ~/.Tox-Backup-$now
	echo "Done! You profile(s) have been moved! A back up coppy still exists at:"
	echo "$bak"
else
	echo "Cannot locate old profile directory, profile migration not performed"
fi
exit 0
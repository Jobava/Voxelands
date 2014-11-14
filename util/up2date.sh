#!/bin/sh

##########################################################################
# up2date.sh
# voxelands - 3d voxel world sandbox game
# Copyright (C) Lisa Milne 2014 <lisa@ltmnet.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
##########################################################################
# Automatically check to see which repositories have older versions of voxelands
#
# To use, execute this script with ./up2date.sh
# Parameters: 1) 
#
# Inspired by up2date=() used with pacman-g2 FrugalBuild scripts
# http://frugalware.org/docs/stable/makepkg.html#_a_simple_example
##########################################################################

###
# == GETOPT STUFF
# * Some derived from https://gist.github.com/hoylen/6607180
# which is public domain material
###
PROG=$(basename $0)

getopt -T > /dev/null
if [ $? -eq 4 ]; then
	# GNU enhanced getopt is available
	ARGS=$(getopt --name "$PROG" --long help,sanity-off --options hs -- "$@")
else
	# Original getopt is available (no long option names, no whitespace, no sorting)
	ARGS=$(getopt hs "$@")
fi

if [ $? -ne 0 ]; then
	echo "$PROG: usage error (use -h for help)" >&2
	exit 2
fi

eval set -- "$ARGS"

while [ $# -gt 0 ]; do
	case "$1" in
		-h | --help)		HELP=yes;;
		-s | --sanity-off)	SANITY_OFF=yes;;
		--)			shift; break;; # end of options
	esac
	shift
done

if [ $# -gt 0 ]; then
	# Remaining parameters can be processed
	for ARG in "$@"; do
		echo "$PROG: argument: $ARGS"
	done
fi

###
# * Help text
###
if [[ $HELP = yes ]]; then
	echo "Allowed options:"
	echo "  -h, --help"
	echo "      Show this help message and exit"
	echo "  -s, --sanity-off"
	echo "      Don't do sanity checks, nice if they're throwing warnings for no good reason"
	exit 0
fi

###
# == DEPENDANCY CHECK
# * Make sure needed programs are installed
###
depends=('lynx')

for i in ${depends[@]}
do
	# This assumes all programs listed in depends can be executed with --version
	eval $i --version > /dev/null 2>&1
	if [[ $? != 0 ]]; then
		echo "$i not found! You need to install $i in order to use this script"
		exit 1
	fi
done

###
# == REPO COMMANDS
# Use a command that will fetch the current version of voxelands in a repository.
# The next tidbit is from Frugalware docs: http://frugalware.org/docs/stable/makepkg.html#_a_simple_example
# Which is licensed under the standard GNU free documentation license: http://frugalware.org/docs/license
#
# lynx -dump http://www.kyz.uklinux.net/cabextract.php |grep 'cabextract source code' |sed 's/.*-\(.*\).t.*/\1/'
# Usually the command consists of three parts: a lynx -dump someurl, a grep foo, and a sed command. We use the http protocol
# if possible, but sometimes we have to use ftp. In that case instead of lynx -dump you should use wget -O - -q.
# Of course, you could use wget all the time, but lynx is simpler. The sed command could be replaced with the combination
# of tr and cut if you prefer them instead of sed. The example used above would be the following with cut and tr:
# lynx -dump http://www.kyz.uklinux.net/cabextract.php |grep 'cabextractsource code'|tr -s ' '|cut -d ' ' -f 6
###
latestVersion="$(lynx --dump https://gitorious.org/minetest-classic/minetest-classic/raw/doc/changelog.txt \
		| grep release | tr '\n' ' ' | cut -d ':' -f1)"
repo=(
	"source"
	"$(lynx -dump http://voxelands.com/download.html | grep src | cut -d '-' -f2)"

	"windows"
	"$(lynx -dump http://voxelands.com/download.html | grep win | cut -d '-' -f2)"

	"osx"
	"$(lynx -dump http://voxelands.com/download.html | grep osx | cut -d '-' -f2)"

	"aur"
	"$(lynx --dump https://aur.archlinux.org/packages/vo/voxelands/PKGBUILD | grep pkgver= | cut -d '=' -f2)"

	"ccr"
	"$(lynx --dump http://chakraos.org/ccr/pkgbuild_view.php?p=voxelands | grep pkgver= | cut -d '=' -f2)"

	"copr"
	"$(lynx --dump http://copr.fedoraproject.org/coprs/mavjs/voxelands/builds/ \
	| grep 'succeeded' | tr '\n' ' ' | tr -s ' ' | cut -d ' ' -f4)"

	"slackbuilds"
	"$(lynx --dump http://slackbuilds.org/repository/14.1/games/voxelands/ \
	| grep 'Games > voxelands' | tr ')' '(' | cut -d '(' -f2)"
)

###
# == PROVIDED FUNCTIONS
# * RepoSanityCheck(): Tries to warn you when a repo command is broken.
# Parameters: 1) repo name 2) version of voxelands in said repo
###
RepoSanityCheck() {
	if [[ -z "$2" ]]; then
		echo "$PROG: ERROR: fetching $1 version failed, repo command returned nothing"
	elif [[ ${#2} != 7 ]]; then
		# voxelands version numbers are typically seven characters long
		echo "$PROG: WARNING: $1 version is not the typical seven character length. Returned: \"$2\""
	fi

	if [[ ! -z "$(echo "$2" | sed 's/[0-9.]//g')" ]] ; then
		echo "$PROG: WARNING: $1 version contains non-numerical non-period characters. Returned: \"$2\""
	fi
}

###
# == PARSE RESULTS
# * Miscellaneous setup
###
if [[ ! $SANITY_OFF = yes ]]; then
	RepoSanityCheck "latest" "$latestVersion"
fi
echo "voxelands latest is $latestVersion"
allUp2Date=true
nOutOfDateRepos=0

###
# * Parse $repo
###
for (( i = 1; i < "${#repo[@]}"; i = i + 2 ))
do
	if [[ ! $SANITY_OFF = yes ]]; then
		RepoSanityCheck ${repo[(( i - 1 ))]} "${repo[$i]}"
	fi
	if [[ "${repo[$i]}" != "$latestVersion" ]]; then
		echo "voxelands ${repo[(( i - 1 ))]} is ${repo[$i]}"
		allUp2Date=false
		(( nOutOfDateRepos++ ))
	fi
done

###
# * Print a status summery
###
if [[ $allUp2Date = true ]]; then
	echo "All known repos have voxelands latest"
elif (( $nOutOfDateRepos < "${#repo[@]}" / 2 )); then
	echo "Other known repos have voxelands latest"
else
	echo "All known repos have older versions of voxelands"
fi

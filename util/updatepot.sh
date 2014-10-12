#!/bin/sh

#
# Update/create voxelands po and pot files
#
# To update only the pot file, execute this script with `./updatepot.sh`.
# You should do this with each modification, addition, or deletion of a
# translatable string in the source code, to give translators the most amount
# of time possible to translate it before the next release. (Transifex
# automatically pulls that file)
#
# To update the pot file and source texts in the po files, execute this script
# with `./updatepot.sh --po`. Though so long as we use Transifex, we shouldn't
# need to use this option.
#
# To start a localization for a new language, create a directory for it in po/
# named as it's language code, then execute this script with
# `./updatepot.sh --po`. But again, as long as we use Transifex, we shouldn't
# need to use this option.
#
# In the above examples, I execute this script from the current directory,
# however it can actually be executed from any directory.
#
# Because we use the --package-name flag, you must have xgettext>=0.17 for
# this script to work. Use `xgettext -V` to check which version you have.
#

# an auxiliary function to abort processing with an optional error message
abort() {
	test -n "$1" && echo >&2 "$1"
	exit 1
}

# If flags exist, and are written incorrectly, abort.
# Because this is currently the only flag available, I simply used a couple
# if statements to make it work. However in the future, if we want more
# options, we may need to switch to something more advanced, like getopt.
if [[ -n $1 ]] && [[ ! $1 == "--po" ]]; then
	abort "'$1' is not a valid option"
fi

# The po/ directory is assumed to be parallel to the directory where
# this script is. Relative paths are fine for us so we can just
# use the following trick (works both for manual invocations and for
# script found from PATH)
scriptisin="$(dirname "$(which "$0")")"

# Commands below are executed from the parent of po/, which is also the
# parent of this script's directory and of the src/ directory.
# We go through $scriptisin so that this script can be executed from whatever
# directory and still work correctly
cd "$scriptisin/.."

test -e po || abort "po/ directory not found"
test -d po || abort "po/ should be a directory, but is not!"

# Create a list of the languages we have to update/create.
# This assumes that we won't have dirnames with spaces, which is
# the case for language codes, which are the only subdirs we expect to
# find in po/ anyway. If you put anything else there, you need to suffer
# the consequences of your actions, so we don't do sanity checks
cd po || abort "couldn't change directory to po!"

langs=""

for lang in * ; do
	if test ! -d $lang; then
		continue
	fi
	langs="$langs $lang"
done

# First thing first, update the .pot template. We place it in the po/
# directory at the top level.
echo "updating the pot file"

# go to parent dir of po/ and src/
cd ..

potfile=po/voxelands.pot
xgettext --package-name=voxelands --copyright-holder="Lisa 'darkrose' Milne" -kN_ -kwgettext -F -n -o $potfile src/*.cpp src/*.h

# We just updated the pot file, now update po files if --po was specified.
if [[ $1 == "--po" ]]; then
	# Now iterate on all language dirs in po/ and create the po file if
	# nonexistent, or update it if it exists already
	for lang in $langs ; do # note the missing quotes around $langs
		pofile=po/$lang/voxelands.po
		if test -e $pofile; then
			echo "[$lang]: updating strings"
			msgmerge -F -U $pofile $potfile
		else
			# This creates a new po file and asks for the translator's identity
			echo "[$lang]: creating $lang localization files"
			msginit -l $lang -o $pofile -i $potfile
		fi
	done
fi
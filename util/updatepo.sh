#!/bin/sh

#
# Update/create voxelands po files
# 
# To update source texts in the po files, simply execute this script with
# `./updatepo.sh`
# 
# To start a localization for a new language, create a directory for it in po/
# named as it's language code, then execute this script with `./updatepo.sh`
# 
# Because we use the --package-name flag, you must have xgettext>=0.17 for
# this script to work. Use `xgettext -V` to check which version you have.
# 

# an auxiliary function to abort processing with an optional error message
abort() {
        test -n "$1" && echo >&2 "$1"
        exit 1
}

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

# go to parent dir of po/ and src/
cd ..

# First thing first, update the .pot template. We place it in the po/
# directory at the top level.
potfile=po/voxelands.pot
xgettext --package-name=voxelands --copyright-holder="Lisa 'darkrose' Milne" -kN_ -kwgettext -F -n -o $potfile src/*.cpp src/*.h

# Now iterate on all language dirs and create the po file if missing, or update it
# if it exists already
for lang in $langs ; do # note the missing quotes around $langs
        pofile=po/$lang/voxelands.po
        if test -e $pofile; then
                echo "[$lang]: updating strings"
                msgmerge -F -U $pofile $potfile
        else
                # This will ask for the translator identity
                echo "[$lang]: creating $lang localization files"
                msginit -l $lang -o $pofile -i $potfile
        fi
done

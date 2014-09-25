#!/usr/bin/env sh

tmp="`pwd`"
proj_dir=`dirname "$tmp"`
mac_dir="$proj_dir/misc/mac"
plist="$mac_dir/Info.plist"
sed=$(which sed)
$sed -i -e "s/GIT_VERSION/$1/" "$plist"

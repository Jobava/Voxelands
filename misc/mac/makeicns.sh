#!/usr/bin/env sh

set -x
tmp="`pwd`"
proj_dir=`dirname "$tmp"`
mac_dir="$proj_dir/misc/mac"
iconset_dir="$mac_dir/voxelands.iconset"
orig_img="$mac_dir/voxelands-1024x1024.png"
sizes=(16 32 64 128 256 512)
sips=$(which sips)
iconutil=$(which iconutil)
inkscape="/Applications/Inkscape.app/Contents/Resources/bin/inkscape"
if [ ! -f "$mac_dir"/voxelands-1024x1024.png ]; then
    "$inkscape" --export-png "$orig_img" -w 1024 -h 1024 "$proj_dir"/misc/freedesktop/icons/scalable/apps/voxelands.svg
fi
mkdir "$iconset_dir"
"$sips" -z "${sizes[0]}" "${sizes[0]}" "$orig_img" --out "$iconset_dir"/icon_"${sizes[0]}"x"${sizes[0]}".png
"$sips" -z "${sizes[1]}" "${sizes[1]}" "$orig_img" --out "$iconset_dir"/icon_"${sizes[0]}"x"${sizes[0]}"@2x.png
"$sips" -z "${sizes[1]}" "${sizes[1]}" "$orig_img" --out "$iconset_dir"/icon_"${sizes[1]}"x"${sizes[1]}".png
"$sips" -z "${sizes[2]}" "${sizes[2]}" "$orig_img" --out "$iconset_dir"/icon_"${sizes[1]}"x"${sizes[1]}"@2x.png
"$sips" -z "${sizes[3]}" "${sizes[3]}" "$orig_img" --out "$iconset_dir"/icon_"${sizes[3]}"x"${sizes[3]}".png
"$sips" -z "${sizes[4]}" "${sizes[4]}" "$orig_img" --out "$iconset_dir"/icon_"${sizes[3]}"x"${sizes[3]}"@2x.png
"$sips" -z "${sizes[4]}" "${sizes[4]}" "$orig_img" --out "$iconset_dir"/icon_"${sizes[4]}"x"${sizes[4]}".png
"$sips" -z "${sizes[5]}" "${sizes[5]}" "$orig_img" --out "$iconset_dir"/icon_"${sizes[4]}"x"${sizes[4]}"@2x.png
"$sips" -z "${sizes[5]}" "${sizes[5]}" "$orig_img" --out "$iconset_dir"/icon_"${sizes[5]}"x"${sizes[5]}".png
cp "$orig_img" "$iconset_dir"/icon_"${sizes[5]}"x"${sizes[5]}"@2x.png
"$iconutil" -c icns "$iconset_dir"

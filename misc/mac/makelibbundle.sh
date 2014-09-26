#!/usr/bin/env sh

tmp=`pwd`
proj_dir=`dirname "$tmp"`
applibs_dir="$proj_dir/libs/"
bin="$1/voxelands"
dylibs=($(otool -L "$bin" | grep '/usr/local' | awk -F" " '{print $1}'))
mkdir "$applibs_dir"
for lib in "${dylibs[@]}"
do
   cp "$lib" "$applibs_dir"
    echo "############################################################################################"
    echo "#####                                    Bundling Libs                                 #####"
    echo "############################################################################################"
    echo "#### install_name_tool -change "$lib" @executable_path/../Resources/libs/$(basename $lib) "$bin" #####"
    echo "############################################################################################"
    install_name_tool -change "$lib" @executable_path/../Resources/libs/$(basename $lib) "$bin"
done

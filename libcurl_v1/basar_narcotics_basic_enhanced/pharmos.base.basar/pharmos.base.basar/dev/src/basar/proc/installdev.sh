#!/bin/bash -e

src_dir=$(pwd)
install_dir=${src_dir}/pkg-export

project=ssuk
version=$(xmllint --xpath "string(//ivy-module/info/@revision)" $src_dir/ivy.xml | sed -e 's/-${platform}//g' )

runtime_dir=${WSS}/lib/marathon/$project-$version

echo "Installing $project files into $runtime_dir" >&2

rsync -av $install_dir/ $runtime_dir/

ln -sf ../../data/basar.config.xml $runtime_dir/sharedlib/debug/
ln -sf ../../data/basar.config.xml $runtime_dir/sharedlib/release/

exit 0

#!/usr/bin/bash -e

top_dir=$(pwd)/..
src_dir=$(pwd)
ext_dir=${top_dir}/../ext
package_name=ssuk
export_dir=${top_dir}/../../export/${package_name}

echo "exporting ${package_name} to ${export_dir}..." >&2

rm -rf ${top_dir}/pkg-export
mkdir -p ${top_dir}/pkg-export

pushd ${top_dir}/pkg-export >/dev/null

echo "-- copying basar ..." >&2

echo "   - include files" >&2
mkdir -p basar/include
rsync -a --link-dest=${src_dir}/library/include/ ${src_dir}/library/include/ basar/include/

echo "   - shared libraries" >&2
mkdir -p sharedlib/{Debug,Release}
ln -s Debug   sharedlib/debug
ln -s Release sharedlib/release

cp -h ${src_dir}/library/lib/debug/*.so*   sharedlib/Debug/
cp -h ${src_dir}/library/lib/release/*.so* sharedlib/Release/

echo "" >&2
echo "-- copying 3rd-party dependencies ..." >&2

echo "   - boost" >&2

#mkdir -p boost/include
#rsync -a --link-dest=${ext_dir}/boost/include/ ${ext_dir}/boost/include/ boost/include/

cp -h ${ext_dir}/boost/lib/*.so*   sharedlib/Debug/
cp -h ${ext_dir}/boost/lib/*.so* sharedlib/Release/

echo "   - ghostscript" >&2

#mkdir -p ghostscript/include
#rsync -av --link-dest=${ext_dir}/ghostscript/include/ ${ext_dir}/ghostscript/include/ ghostscript/include/

mkdir -p sharedlib/ghostscript
rsync -a --link-dest=${ext_dir}/ghostscript/ghostscript/ ${ext_dir}/ghostscript/ghostscript/ sharedlib/ghostscript/

cp -h ${ext_dir}/ghostscript/lib/libgs.so* sharedlib/Debug/
cp -h ${ext_dir}/ghostscript/lib/libgs.so* sharedlib/Release/

echo "   - libssh2" >&2

#mkdir -p libssh2/include
#rsync -a --link-dest=${ext_dir}/libssh2/include/ ${ext_dir}/libssh2/include/ libssh2/include/

cp -h ${ext_dir}/libssh2/lib/debug/*.so*   sharedlib/Debug/
cp -h ${ext_dir}/libssh2/lib/release/*.so* sharedlib/Release/

echo "   - log4cplus" >&2

#mkdir -p log4cplus/include
#rsync -a --link-dest=${ext_dir}/log4cplus/include/ ${ext_dir}/log4cplus/include/ log4cplus/include/

cp -h ${ext_dir}/log4cplus/lib/debug/*.so*   sharedlib/Debug/
cp -h ${ext_dir}/log4cplus/lib/release/*.so* sharedlib/Release/

echo "   - openssl" >&2

#mkdir -p openssl/include
#rsync -a --link-dest=${ext_dir}/openssl/include/ ${ext_dir}/openssl/include/ openssl/include/

cp -h ${ext_dir}/openssl/lib/*.so* sharedlib/Debug/
cp -h ${ext_dir}/openssl/lib/*.so* sharedlib/Release/

# We need to copy .a files here too
cp -h ${ext_dir}/openssl/lib/*.a* sharedlib/Debug/
cp -h ${ext_dir}/openssl/lib/*.a* sharedlib/Release/

echo "   - openldap" >&2

#mkdir -p openssl/include
#rsync -a --link-dest=${ext_dir}/openssl/include/ ${ext_dir}/openssl/include/ openssl/include/

cp -h ${ext_dir}/openldap/lib/*.so* sharedlib/Debug/
cp -h ${ext_dir}/openldap/lib/*.so* sharedlib/Release/

echo "   - qt5" >&2

#mkdir -p qt/include
#rsync -a --link-dest=${ext_dir}/qt5/include/ ${ext_dir}/qt5/include/ qt/include/

for qt_lib in Concurrent Core DBus Ftp Network Sql Test Xml
do
    #debug
    cp -h ${ext_dir}/qt5/lib/libQt5${qt_lib}d*.so* sharedlib/Debug/
    cp -h ${ext_dir}/qt5/lib/libQt5${qt_lib}d*.a   sharedlib/Debug/

    #release
    cp -h ${ext_dir}/qt5/lib/libQt5${qt_lib}*.so* sharedlib/Release/
    cp -h ${ext_dir}/qt5/lib/libQt5${qt_lib}*.a   sharedlib/Release/
done

mkdir -p sharedlib/multilang
cp -h ${ext_dir}/qt5/translations/*.qm sharedlib/multilang/

echo "   - zlib" >&2

#mkdir -p zlib/include
#rsync -a --link-dest=${ext_dir}/zlib/include/ ${ext_dir}/zlib/include/ zlib/include/

cp -h ${ext_dir}/zlib/lib/debug/*.so*   sharedlib/Debug/
cp -h ${ext_dir}/zlib/lib/release/*.so* sharedlib/Release/

echo "" >&2

echo "-- copying basar/proc ..." 2>&2

mkdir -p proc
cp ${src_dir}/proc/deploy*.sh       proc/
cp ${src_dir}/proc/install*.sh      proc/

echo "-- copying basar/data ..." 2>&2

mkdir -p data
cp ${src_dir}/data/version.inc      data/
cp ${src_dir}/data/basar.config.xml data/

popd >&2

mkdir -p ${export_dir}

tar -C ${top_dir}/pkg-export -cvzf ${export_dir}/${package_name}.tar.gz .

cp ${top_dir}/ivy.xml ${export_dir}
cp ${top_dir}/build.xml ${export_dir}

echo "done" >&2
echo "" >&2

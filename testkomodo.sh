#!/bin/bash
GIT=/prog/sdpsoft/bin/git

source /opt/rh/devtoolset-8/enable
GCC_VERSION=7.3.0 CMAKE_VERSION=3.10.2 source /prog/sdpsoft/env.sh
set -e

echo "building c tests"
rm -rf build
mkdir build
pushd build
cmake .. -DBUILD_TESTS=ON\
-DENABLE_PYTHON=OFF\
-DBUILD_APPLICATIONS=ON\
-DCMAKE_INSTALL_PREFIX=install\
-DCMAKE_PREFIX_PATH=/project/res/komodo/bleeding/root/\
-DCMAKE_C_FLAGS='-Werror=all'\
-DCMAKE_CXX_FLAGS='-Wno-unused-result'
make -j 12
#removing built libs in order to ensure we are using libs from komodo
rm -r lib64

echo "running ctest"
ctest --output-on-failure

popd

echo "running pytest"
#run in a new folder so that we dont load the other python code from the source, but rather run against komodo
rm -rf tmptest
mkdir tmptest
cp -r python/tests tmptest/tests
pushd tmptest
python -m pytest

popd


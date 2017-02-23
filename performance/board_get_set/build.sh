#!/bin/bash
compiler=clang++

if [ ! -d build ]; then
	mkdir build
fi

pushd build

cmake -DCMAKE_CXX_COMPILER=${compiler} -DCMAKE_BUILD_TYPE=Release ..
make -j ${MAKE_JOB_NUMBER}

popd


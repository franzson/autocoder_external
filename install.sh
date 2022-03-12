#!/bin/bash
mkdir "$1/Packages/autocoder"
mkdir "$1/Packages/autocoder/externals"
echo "   ...   installing the external into $1/Packages/autocoder/externals   ..."
cp -R ./autocoder.mxo "$1/Packages/autocoder/externals"


if [ -d "/usr/local/lib" ]; then
    echo "   ...   /usr/local/lib already exists   ..."
else
    echo "   ...   creating /usr/local/lib   ..."
    sudo mkdir /usr/local/lib
fi
echo "   ...   installing tensorflow lite into /usr/local/lib   ... "
cp ./libtensorflowlite_c.dylib /usr/local/lib

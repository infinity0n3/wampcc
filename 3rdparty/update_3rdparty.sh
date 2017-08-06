#!/usr/bin/env bash

##
## if you are behind a firewall, set a proxy
##
##    export http_proxy=http://your_ip_proxy:port/
##    export https_proxy=$http_proxy
##

THIRD_PARTY_DIR=./

##
## websocketpp
##

websocketpp_ver=0.7.0
echo '***' fetching websocketpp_ver $websocketpp_ver '***'
echo
tarfile=websocketpp-${websocketpp_ver}.tar.gz
test -f $tarfile || wget https://github.com/zaphoyd/websocketpp/archive/${websocketpp_ver}.tar.gz  -O $tarfile

if [ -f ${tarfile} ]; then
    rm -rf ${THIRD_PARTY_DIR}/websocketpp
    tar xfz  ${tarfile} -C ${THIRD_PARTY_DIR}  --transform "s/^websocketpp-${websocketpp_ver}/websocketpp/"
    rm ${tarfile}
else
  echo failed to download websocketpp ... please try manually
fi


##
## msgpack
##

# Need version >= 2.1.2, because for 2.1.2 and earlier, it has a bug in the
# decoding of msgpack buffers
# version 2.1.5 does not compile
ver=2.1.3
echo '***' fetching msgpack $ver '***'
echo
zipfile=cpp-${ver}.tar.gz
url=https://github.com/msgpack/msgpack-c/archive/$zipfile
test -f $zipfile || wget $url

if [ -f ${zipfile} ]; then
    rm -rf ${THIRD_PARTY_DIR}/msgpack-c
    tar xfz  ${zipfile} -C ${THIRD_PARTY_DIR}
    mv ${THIRD_PARTY_DIR}/msgpack-c-cpp-${ver} ${THIRD_PARTY_DIR}/msgpack-c
    rm ${zipfile}
else
    echo failed to download msgpack ... please try manually
    exit
fi

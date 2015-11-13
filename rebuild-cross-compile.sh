#!/bin/bash
set -e

if [ $# != 1 ]; then
  echo "Usage: $0 <OpenWrt-SDK-dir>"
  exit 1;
fi

sdk_dir=$1
#sdk_dir=/home/chenzp/openwrt/chaos_calmer

toolchain_dir=`find $sdk_dir/staging_dir -maxdepth 1 -name "toolchain-*"`
target_dir=`find $sdk_dir/staging_dir -maxdepth 1 -name "target-*"`
export PATH=$PATH:$sdk_dir/staging_dir/host/bin:${toolchain_dir}/bin
export STAGING_DIR=$sdk_dir/staging_dir
echo PATH=$PATH
echo STAGING_DIR=$STAGING_DIR

arch=$(ls ${toolchain_dir}/bin | grep -Po '^(.*)(?=-openwrt-linux-gcc$)')

./configure --build=`./config.guess` --host="${arch}-openwrt-linux" CC="${arch}-openwrt-linux-gcc" LD="${arch}-openwrt-linux-ld" CFLAGS="-O2 -I${target_dir}/usr/include -D_GNU_SOURCE" LDFLAGS="-L${target_dir}/usr/lib"

make clean
make


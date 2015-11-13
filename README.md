[![Build Status](https://travis-ci.org/pengeorge/TUNet-OpenWrt.svg?branch=master)]
(https://travis-ci.org/pengeorge/TUNet-OpenWrt)

TUNet-OpenWrt
=============

Tunet program for OpenWrt

Installation
------------

```shell
./rebuild-cross-compile.sh <OpenWrt-SDK-directory>
```

Usage
-----

Usage: tunet [options] `<`login|logout|info|status`>`

       -c: the configuration file, default: /etc/tunet/tunet.conf

       -uUSERNAME: set username as USERNAME"

       -pPASSWORD: set password as PASSWORD"


Example of config file
----------------------

```perl
host_ip = 166.111.204.120 # optional  
port = 80 #optional  
username = myname15  
password = 123456  
```


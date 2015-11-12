#!/bin/bash

aclocal
automake --add-missing
autoheader
sed -i 's:\(^#undef malloc$\|^#undef realloc$\):/* \1 */:' config.h.in
autoconf


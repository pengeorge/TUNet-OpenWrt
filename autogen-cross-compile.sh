#!/bin/bash

aclocal
autoheader
automake --add-missing
sed -i 's:\(^#undef malloc$\|^#undef realloc$\):/* \1 */:' config.h.in
autoconf


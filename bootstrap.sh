#!/usr/bin/env sh

autoreconf --install
automake --add-missing --copy
# automake --add-missing --copy >/dev/null 2>&1

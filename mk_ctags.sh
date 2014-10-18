#!/bin/sh

ctags -e -R --append -f TAGS.NEW --languages=C,C++ --exclude=SVN-TEST\* . && \
    mv TAGS.NEW TAGS

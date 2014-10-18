CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_SKIP_ASSEMBLY_SOURCE_RULES TRUE)

PROJECT(git-isvn)

# APR and SVN are garbage. Make sure we have headers:
FIND_PATH(APR_INCL_DIR apr.h /usr/include/apr-1)
FIND_PATH(SVN_INCL_DIR svn_client.h /usr/include/subversion-1)

INCLUDE_DIRECTORIES(${APR_INCL_DIR} ${SVN_INCL_DIR} .)

# Project files:
FILE(GLOB_RECURSE SRC_GIT_ISVN src/*.c src/*.h)

# Imported from git/vcs-svn/ (BSD-licensed):
FILE(GLOB_RECURSE SRC_VCS_SVN vcs-svn/*.c vcs-svn/*.h)

ADD_DEFINITIONS(-D_GNU_SOURCE)

SET(CMAKE_C_FLAGS "-fstack-protector-strong --param=ssp-buffer-size=4 -grecord-gcc-switches ${CMAKE_C_FLAGS}")
SET(CMAKE_C_FLAGS "-g -O2 -std=gnu99 -Wall -Wextra -Werror -mtune=generic -pipe -Wp,-D_FORTIFY_SOURCE=2 ${CMAKE_C_FLAGS}")

ADD_EXECUTABLE(git-isvn ${SRC_GIT_ISVN} ${SRC_VCS_SVN})
TARGET_LINK_LIBRARIES(git-isvn git2 svn_client-1 svn_delta-1 svn_ra-1 svn_subr-1 apr-1 crypto ssl bsd pthread z)

git-isvn(1)
===========

NAME
----
git-isvn - Clone-only (WIP) operation between a Subversion repository and Git

SYNOPSIS
--------
`git isvn <command> [options] [arguments]`

DESCRIPTION
-----------
'git isvn' is a fast, bare bones replica of an SVN repository. In the future it
may support bidirectional changes.

'git isvn' is intentionally light on features and knobs.

COMMANDS
--------

### clone

> Initializes an empty Git repository and runs 'fetch'.

    --trunk=<trunk_subdir>
    --branches=<branches_subdir>
    --tags=<branches_subdir>
    --username=<user>
    --password=<password>

BASIC EXAMPLES
--------------

### Clone a repo (like git clone):

`git isvn clone http://svn.example.com/project project`

CAVEATS
-------
As with git-svn, users should avoid Git merging / fetching / pulling while
using git-isvn. Instead, history should be kept as linear as possible. Merges
should be performed seperately with the SVN command line tools.

BUGS
----
Lots.

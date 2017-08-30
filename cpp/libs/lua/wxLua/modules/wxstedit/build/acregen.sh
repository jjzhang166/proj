#!/bin/bash
#
# Author: Francesco Montorsi
# RCS-ID: $Id: acregen.sh 725 2008-03-10 12:22:31Z frm $
# Creation date: 14/9/2005
#
# A simple script to generate the configure script for a wxCode component
# Some features of this version:
# - automatic test for aclocal version
# - able to be called from any folder
#   (i.e. you can call it typing 'build/acregen.sh', not only './acregen.sh')

# You may pass optional args to aclocal on the command line.
# If you are using a local copy (newer perhaps) of bakefile and have a system
# wide bakefile installed you need to force aclocal to use the bakefile*.m4
# files from the local copy and not from /usr/share/aclocal/bakefile*.m4
# E.G. $./acregen.sh "--acdir=/path/to/bakefile-x.y.z/share/aclocal"
ACLOCAL_ARGS=$1

# called when an old version of aclocal is found
function aclocalold()
{
    echo "Your aclocal version is $aclocal_maj.$aclocal_min.$aclocal_rel"
    echo "Your automake installation is too old; please install automake >= $aclocal_minimal_maj.$aclocal_minimal_min.$aclocal_minimal_rel"
    echo "You can download automake from ftp://sources.redhat.com/pub/automake/"
    exit 1
}

# first check if we have an ACLOCAL version recent enough
aclocal_verfull=$(aclocal --version)
aclocal_maj=`echo $aclocal_verfull | sed 's/aclocal (GNU automake) \([0-9]*\).\([0-9]*\).\([0-9]*\).*/\1/'`
aclocal_min=`echo $aclocal_verfull | sed 's/aclocal (GNU automake) \([0-9]*\).\([0-9]*\).\([0-9]*\).*/\2/'`
aclocal_rel=`echo $aclocal_verfull | sed 's/aclocal (GNU automake) \([0-9]*\).\([0-9]*\).\([0-9]*\).*/\3/'`

aclocal_minimal_maj=1
aclocal_minimal_min=9
aclocal_minimal_rel=6

majok=$(($aclocal_maj > $aclocal_minimal_maj))
minok=$(($aclocal_maj == $aclocal_minimal_maj && $aclocal_min > $aclocal_minimal_min))
relok=$(($aclocal_maj == $aclocal_minimal_maj && $aclocal_min == $aclocal_minimal_min && \
         $aclocal_rel >= $aclocal_minimal_rel))

versionok=$(($majok == 1 || $minok == 1 || $relok == 1))
if [[ "$versionok" = "0" ]]; then aclocalold; fi

# we can safely proceed
me=$(basename $0)
path=${0%%/$me}        # path from which the script has been launched
current=$(pwd)
cd $path
aclocal $ACLOCAL_ARGS && autoconf && mv configure ..
cd $current

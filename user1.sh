#!/bin/bash

# Read user ID
if [ $# -ne 1 ]; then
  echo -n "Enter user-name: "
  read userid
else
  userid=$1
fi

# Find line containing user ID
search='^'$userid':'
line=`grep $search passwd1.txt`
if [[ -z $line ]]; then
  echo $userid" not found!"
  exit 1
fi

# Split by ':' and print outputs
set -f
IFS=$'\n' # make newlines the only separator
i=0
for tok in `echo $line | tr ':' '\n'`
do
  i=`expr $i + 1`
  case "$i" in
    1)
      echo 'userid: '$tok
      ;;
    3)
      echo 'uid: '$tok
      ;;
    4)
      echo 'gid: '$tok
      ;;
    6)
      echo 'home directory: '$tok
      ;;
    7)
      echo 'shell: '$tok
      ;;
  esac
done
set +f

exit 0

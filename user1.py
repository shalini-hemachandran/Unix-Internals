#!/usr/bin/python

import sys
import re

# Read user ID
if len(sys.argv) < 2:
  sys.stdout.write('Enter user-name: ')
  userid = sys.stdin.readline().strip('\n')
else:
  userid = sys.argv[1]

with open('passwd1.txt') as f:
  found = False
  lines = f.readlines()
  lines = [x.strip('\n') for x in lines]
  for line in lines:
    pattern = re.compile('^' + userid + ':(.*)')
    matchObj = re.match(pattern, line)
    if matchObj is not None:
      found = True
      matchedLine = matchObj.group(1)
      tokens = matchedLine.split(':')
      print 'userid:', userid
      if len(tokens[1]) > 0:
        print 'uid:', tokens[1]
      if len(tokens[2]) > 0:
        print 'gid:', tokens[2]
      if len(tokens[4]) > 0:
        print 'home directory:', tokens[4]
      if len(tokens[5]) > 0:
        print 'shell:', tokens[5]
  if not found:
    print userid, 'not found!'

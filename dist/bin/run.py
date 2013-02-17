#!/usr/bin/env python
import argparse
import re
import subprocess
import sys

parser = argparse.ArgumentParser()
parser.add_argument("executable", 
	help="find runtime exceptions for a given Ogre executable")
args = parser.parse_args()

subprocess.call(['./' + args.executable,])

print '\n::BEGIN EXCEPTIONS::'
for line in open('Ogre.log'):
  if re.search(r'EXCEPTION', line):
    print line
print '::END EXCEPTIONS::'


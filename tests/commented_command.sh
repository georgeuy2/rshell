#!/bin/sh
ls #test
ls -a #test
echo moose #testing
cd .. #moose
ls #checking directory
mkdir temp #delete this later
ls #show new dir
date #print date
hostname #print hostname
logname #prints login
rm temp #deleting temp
time #print time
test -f /test/file/path #comments
[ -f /test/file/path ] #comment
ls -a || (echo moose && echo dov #echoing stuff)
(time || date) && logname #done
exit #exiting 

#!/bin/sh
ls || exit
ls && exit
exit #comment 
test -e /test/file/path && exit
test -d /test/file/path || exit
[ -f /test/file/path ] && exit
[ -e /test/file/path ] || exit
(cd .. || exit) && echo shouldn't echo
ls && (echo moose && exit)
(exit && echo A) || (echo B  && echo C)
((ls && date) || time) && exit
exit || ((ls || time) && date)
(echo A || echo B) && exit
exit

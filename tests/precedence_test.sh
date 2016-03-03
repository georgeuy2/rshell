#!/bin/sh
echo A && echo B || echo C && echo D
(echo A && echo B) || (echo C && echo D)
cd .. || ls -a && mkdir temp || pwd
((cd .. || ls -a ) && mkdir temp) || pwd
ls && time || date || echo moose
ls && ((time || date) || echo moose)
(echo moose || echo dov) && logname
time && (date && hostname) || logname
(cd .. || echo done) || exit


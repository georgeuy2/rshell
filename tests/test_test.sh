#!/bin/sh
test -e /test/file/path
test -f /test/file/path
test -d /test/file/path
[ -e /test/file/path ]
[ -f /test/file/path ]
[ -e /test/file/path ]
test -e /test/file/path && echo yes
test -f /test/file/path && echo yes
test -d /test/file/path && echo yes
[ -e /test/file/path ] && echo yes
[ -f /test/file/path ] && echo yes
[ -d /test/file/path ] && echo yes
test -e /test/file/path || echo yes
test -f /test/file/path || echo this is boring
test -d /test/file/path || echo I'm tired
[ -e /test/file/path ] || echo I want to go to bed
[ -f /test/file/path ] || echo soon
[ -d /test/file/path ] || ls
ls && test -e /test/file/path
ls && test -e /test/file/path || echo no
echo yes && logname || test -f /test/file/path

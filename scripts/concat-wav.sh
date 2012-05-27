#!/bin/sh
sox $1 -r 44100 -c 2 -s -w /tmp/$$-1.raw
sox $2 -r 44100 -c 2 -s -w /tmp/$$-2.raw
cat /tmp/$$-1.raw /tmp/$$-2.raw > /tmp/$$.raw
sox -r 44100 -c 2 -s -w /tmp/$$.raw $3
rm /tmp/$$*.raw

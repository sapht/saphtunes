for i in /Volumes/Audio/Workspace/songs/*; do cd "$i"; num=$(/bin/ls | perl -ne 'print if /(\.rns|\.record|\.reason)$/' | wc -l | cut -c 8); if [ "x$num" != "x1" ]; then echo "$i"; fi; done;

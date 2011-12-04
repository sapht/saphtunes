#!/bin/bash
FONT="Helvetica"
while getopts t:l:f: option
	do case $option in
		t) LEN=$(($OPTARG*25));;
		l) LABEL="Daniel Hjerth\n$OPTARG";;
		f) FONT="$OPTARG";;
	esac;
done;
shift $(($OPTIND - 1))
FILE="$1"

echo $LABEL;
if [[ "$LABEL" == "" ]]; then
	echo "no label";
	exit 1;
fi

if [[ "$LEN" == "" ]]; then
	echo "no len";
	exit 1;
fi

if [[ "$FILE" == "" ]]; then
	echo "no file";
	exit 1;
fi

convert -gravity southwest -size 1280x720 -background black -fill white -font "$FONT" -pointsize 30 "label:$LABEL" tmp.jpg
ffmpeg -loop_input -vframes "$LEN" -i tmp.jpg -i "$FILE" -y -r 30 -acodec copy -vcodec mpeg4 out.mp4
#LEN=$(($1 * 25))
#LABEL=$2
#INPUT=$3

#convert -gravity southwest -size 1280x720 -background black -fill white -font Helvetica -pointsize 48 "label:Daniel Hjerth\n$LABEL" tmp.jpg
#ffmpeg -loop_input -vframes "$LEN" -i tmp.jpg -i "$INPUT" -y -r 30 -acodec copy -vcodec mpeg4 out.mp4

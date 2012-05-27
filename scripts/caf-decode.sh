#!/bin/bash
# http://www.devdaily.com/mac-os-x/convert-caf-sound-file-aif-aiff-mp3-format
# a script to convert every CAF sound file in the current
# directory to a corresponding AIF sound file.

IFS=$'\n'

# list all CAF files in the current directory.
# (the -1 character in this line is a "one", not an "el")
for INFILE in $(ls -1 *.caf)
do
  # get the base filename by stripping off the ".caf" part
  baseFilename=`basename "${INFILE}" .caf`

  # determine the preliminary output filename
  outfile="${baseFilename}.aif"

  # convert all spaces to hyphens
  outfile=`echo $outfile | tr -s " " "-"`
  echo "Converting \"$INFILE\" to \"$outfile\" ..."

  # now convert the CAF file to an AIF file
  afconvert -f AIFF -d I8 "$INFILE" "$outfile"

done

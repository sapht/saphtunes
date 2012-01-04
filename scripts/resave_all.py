#!/usr/bin/env python

from glob import glob
import os
import subprocess

for record_file in glob("/Volumes/Audio/Workspace/songs/*/*.rns"):
    project_path = os.path.dirname(record_file)
    project_name = os.path.basename(project_path)
    
    print (record_file, project_path, project_name)

    subprocess.call(
        '''osascript ~/Desktop/saphtunes/scripts/a.scpt "%s" "%s"''' % (
            record_file,
            project_name + '.reason'
        ),
        shell=True
    )

    subprocess.call(
        '''cd "%s";
        git rm *.rns;
        git add *.reason;
        git commit -m "AUTOSAVE .rns->.reason"''' % project_path,
        shell=True
    );


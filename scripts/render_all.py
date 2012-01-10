#!/usr/bin/env python

from glob import glob
import os
import subprocess
import sys

def export_all(render_scpt_path, files):
    korv = []

    for song_path in files:
        project_path = os.path.dirname(song_path)
        project_name = os.path.basename(project_path)

        out_file = project_name + '.wav'
        out_path = project_path + '/' + out_file

        if os.path.isfile(out_path):
            if os.stat(song_path).st_mtime < os.stat(out_path).st_mtime:
                continue

        korv.append(
            {'path': song_path,
             'out': out_file}
        )

    print "Rendering %d files: \n=================\n\t%s" % (len(korv), "\n\t".join([k['path'] for k in korv]))

    for k in korv:
        subprocess.call(
            '''osascript %s "%s" "%s"''' % (
                render_scpt_path,
                k['path'],
                k['out']
            ),
            shell=True
        )



if __name__ == "__main__":
    paths = {}
    scpt = os.path.realpath(sys.path[0] + '/../build/render.scpt')

    if not os.path.exists(scpt):
        print "Rendering script does not exist"
        sys.exit(1);

    try:
        paths = { "songs": os.environ['SAPHTUNE_SONG_DIR'] }
    except KeyError:
        print "Please provide an environment"
        sys.exit(1)

    if not paths:
        print "Bad environment"
        sys.exit(1)

    export_all(scpt, glob(paths['songs'] + "/*/*.reason"))

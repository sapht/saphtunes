from glob import glob
import os
import subprocess
import sys

def render_modified(render_scpt_path, files):
    for song_file in files:
        project_path = os.path.dirname(song_file)
        project_name = os.path.basename(project_path)

        out_file = project_name + '.wav'
        out_path = project_path + '/' + out_file

        if os.path.isfile(out_path):
            if os.stat(song_file).st_mtime < os.stat(out_path).st_mtime:
                continue

        print (song_file, project_path, project_name)

        subprocess.call(
            '''osascript %s "%s" "%s"''' % (
                render_scpt_path,
                song_file,
                out_file
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

    render_modified(scpt, glob(paths['songs'] + "/*/*.reason"))

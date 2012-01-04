#!/usr/bin/perl
use File::stat;

sub load_meta($$) {
    $dir = $_[0];
    $meta_file = $_[1];

    print "$dir\n";
    print "$meta_file\n";

    open(META, ">>$meta_file");
    write(META, "[meta]\n");
    write(META, "
}

foreach $song_dir (<songs/*>) {
    if ( ! -f "$song_dir/meta.ini" ) {
        load_meta($song_dir, "$song_dir/meta.ini");
    }
}

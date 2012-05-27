#!/usr/bin/perl
use strict;

sub sh($) {
    return `$_[0]`;
    #print $_[0] . "\n";
}

sub git_remove_submodule($$) {
    my $repo           = $_[0];
    my $submodule_name = $_[1];

    sh "cd $repo && git rm --cached $submodule_name";
    sh "mv $repo/$submodule_name ../oldalbumsong/";
    # TODO: what happens if we have a trailing /?
}

sub git_commit($) {
    my $repo = $_[0];

    sh "cd $repo && git commit -m 'recouple'";
}

sub git_remove_gitmodules($) {
    my $repo = $_[0];

    sh "cd $repo && git rm .gitmodules";
}


sub git_add_symlink($$$) {
    my $repo        = $_[0];
    my $src_path    = $_[1];
    my $target_name = $_[2];

    sh "cd $repo && ln -s $src_path $target_name";
    sh "cd $repo && git add $target_name";
}

sub process_song($$) {
    my $album_name  = $_[0];
    my $song_name   = $_[1];
    my $song_target    = "../../songs/$song_name";

    # TODO: Which comes first?
    git_remove_submodule ($album_name, $song_name);
    git_add_symlink      ($album_name, $song_target, $song_name);
}

sub process_album($) {
    my $album_name = $_[0];
    my $module_dat = `cat $album_name/.gitmodules`;
    while ($module_dat =~ /\[submodule "([a-z0-9-_]+)"\]/g ) {
        process_song($album_name, $1);
    }
    git_remove_gitmodules($album_name);
    #git_commit($album_name);
}

sub main {
    foreach my $album_name ( `ls` ) {
        chomp $album_name;
        process_album($album_name);
    }
}

main;

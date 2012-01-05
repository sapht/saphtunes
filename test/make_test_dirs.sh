#!/bin/bash
ROOT=/tmp/saphtune
rm -fr "$ROOT"
mkdir -p $ROOT
mkdir -p $ROOT/git/{album,songs}
mkdir -p $ROOT/album
mkdir -p $ROOT/songs

# Songs
for i in {11,12,13,14,21,22,23,24,25,31,32,33,40}; do
    path="$ROOT/songs/song_$i";
    origin="$ROOT/git/songs/song_$i.git"
    git init $path
    git init --bare $origin

    cd $path
    git remote add origin $origin
    touch README
    git add README
    git commit -a -m 'First commit'
    git push origin master
done;

# Albums
for i in {1,2,3}; do
    path="$ROOT/album/album_$i";
    origin="$ROOT/git/album/album_$i.git"
    git init $path
    git init --bare $origin

    cd $path
    git remote add origin $origin
    touch README
    git add README
    git commit -a -m 'First commit'
    for z in "$ROOT/git/songs/song_$i"*; do
        git submodule add "$z"
    done;
    git push origin master
done;

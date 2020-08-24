#!/usr/bin/env bash

# The arguments are all dose files.

while (( "$#" )) ; do

    thefile="$1"

    echo -n $thefile | sed -e 's!.*\([0-9][0-9][0-9]\).*!\1!'
    echo -n ' '

  #Now grab a whole 'swack' of measurements so that we can average them.

    #Each image file contains 5 header lines followed by 60 rows (and 60 columns, but RGB for each)
    cat $thefile | tail -n 60 | cut -d ' ' -f 91 | head -n 20 | tail -n 1 | tr '\n' ' ' | tr '\r' ' ' 
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 91 | head -n 21 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 91 | head -n 22 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 91 | head -n 23 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 91 | head -n 24 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 91 | head -n 25 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 91 | head -n 26 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 91 | head -n 27 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 91 | head -n 28 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 91 | head -n 29 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 91 | head -n 30 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '





    cat $thefile | tail -n 60 | cut -d ' ' -f 94 | head -n 20 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 94 | head -n 21 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 94 | head -n 22 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 94 | head -n 23 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 94 | head -n 24 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 94 | head -n 25 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 94 | head -n 26 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 94 | head -n 27 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 94 | head -n 28 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 94 | head -n 29 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 94 | head -n 30 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '





    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 20 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 21 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 22 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 23 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 24 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 25 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 26 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 27 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 28 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 29 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 30 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '




    cat $thefile | tail -n 60 | cut -d ' ' -f 97 | head -n 20 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 97 | head -n 21 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 97 | head -n 22 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 97 | head -n 23 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 97 | head -n 24 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 97 | head -n 25 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 97 | head -n 26 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 97 | head -n 27 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 97 | head -n 28 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 97 | head -n 29 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 97 | head -n 30 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '




    cat $thefile | tail -n 60 | cut -d ' ' -f 100 | head -n 20 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 100 | head -n 21 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 100 | head -n 22 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 100 | head -n 23 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 100 | head -n 24 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 100 | head -n 25 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 100 | head -n 26 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 100 | head -n 27 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 100 | head -n 28 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 100 | head -n 29 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 100 | head -n 30 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '







    cat $thefile | tail -n 60 | cut -d ' ' -f 103 | head -n 20 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 103 | head -n 21 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 103 | head -n 22 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 103 | head -n 23 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 103 | head -n 24 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 103 | head -n 25 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 103 | head -n 26 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 103 | head -n 27 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 103 | head -n 28 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 103 | head -n 29 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 103 | head -n 30 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '






    cat $thefile | tail -n 60 | cut -d ' ' -f 85 | head -n 20 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 85 | head -n 21 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 85 | head -n 22 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 85 | head -n 23 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 85 | head -n 24 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 85 | head -n 25 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 85 | head -n 26 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 85 | head -n 27 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 85 | head -n 28 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 85 | head -n 29 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 85 | head -n 30 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '





    cat $thefile | tail -n 60 | cut -d ' ' -f 82 | head -n 20 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 82 | head -n 21 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 82 | head -n 22 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 82 | head -n 23 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 82 | head -n 24 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 82 | head -n 25 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 82 | head -n 26 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 82 | head -n 27 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 82 | head -n 28 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 82 | head -n 29 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 82 | head -n 30 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '





    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 30 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 31 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 32 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 33 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 34 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 35 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 36 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 37 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 38 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 39 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '

    cat $thefile | tail -n 60 | cut -d ' ' -f 88 | head -n 40 | tail -n 1 | tr '\n' ' ' | tr '\r' ' '
    echo -n ' '









    echo ' '
    shift
done





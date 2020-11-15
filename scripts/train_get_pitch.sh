#!/bin/bash

# Put here the program (maybe with path)
GETF0="get_pitch"
<<COMMENT
for weight1 in 0.85 0.86 0.87 0.88 0.89 0.9; do
    echo "weight1 = $weight1"
    for fwav in pitch_db/train/*.wav; do
        ff0=${fwav/.wav/.f0}
        $GETF0 $fwav $ff0 -1 $weight1 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
    done
    pitch_evaluate pitch_db/train/*f0ref | grep 'TOTAL'
done
COMMENT
#<<COMMENT
for weight2 in 0.5 0.55 0.6 0.65 0.7 0.73 0.75 0.77; do
    echo "weight2 = $weight2"
    for fwav in pitch_db/train/*.wav; do
        ff0=${fwav/.wav/.f0}
        $GETF0 $fwav $ff0 -2 $weight2 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
    done
    pitch_evaluate pitch_db/train/*f0ref | grep 'TOTAL'
done
#COMMENT
<<COMMENT
for weight3 in  -60 -55 -50 -48 -46 -44 -40; do
    echo "weight3 = $weight3"
    for fwav in pitch_db/train/*.wav; do
        ff0=${fwav/.wav/.f0}
        $GETF0 $fwav $ff0 -3 $weight3 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
    done
    pitch_evaluate pitch_db/train/*f0ref | grep 'TOTAL'
done
COMMENT
<<COMMENT
for weight4 in 0.3 0.35 0.4 0.45 0.5 0.55 0.6; do
    echo "weight4 = $weight4"
    for fwav in pitch_db/train/*.wav; do
        ff0=${fwav/.wav/.f0}
        $GETF0 $fwav $ff0 -4 $weight4 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
    done
    pitch_evaluate pitch_db/train/*f0ref | grep 'TOTAL'
done
COMMENT
<<COMMENT
for weight5 in 0.2 0.25 0.3 0.35 0.4 0.45; do
    echo "weight5 = $weight5"
    for fwav in pitch_db/train/*.wav; do
        ff0=${fwav/.wav/.f0}
        $GETF0 $fwav $ff0 -5 $weight5 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
    done
    pitch_evaluate pitch_db/train/*f0ref | grep 'TOTAL'
done
COMMENT
exit 0

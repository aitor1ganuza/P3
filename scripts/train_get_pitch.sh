#!/bin/bash

# Put here the program (maybe with path)
GETF0="get_pitch"

for weight1 in 0.8 0.85 0.9 0.91 0.92 0.93 0.94 0.95; do
    echo "weight1 = $weight1"
    for fwav in pitch_db/train/*.wav; do
        ff0=${fwav/.wav/.f0}
        $GETF0 $fwav $ff0 -1 $weight1 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
    done
    pitch_evaluate pitch_db/train/*f0ref | grep 'TOTAL'
done

: '
for weight2 in 0.7 0.73 0.75 0.77 0.8; do
    echo "weight2 = $weight2"
    for fwav in pitch_db/train/*.wav; do
        ff0=${fwav/.wav/.f0}
        $GETF0 $fwav $ff0 -2 $weight2 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
    done
    pitch_evaluate pitch_db/train/*f0ref | grep 'TOTAL'
done
'
exit 0

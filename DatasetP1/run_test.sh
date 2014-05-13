#!/bin/bash

tests=19
if [[ -n $1 ]]; then
    tests=$1
fi
range=0..`echo $tests`

for i in $(eval echo {$range})
do
        cp DatasetP1/input/input$i.txt input.txt
        echo -n "$i: "
        (time ./generali) &> /tmp/res && cat /tmp/res | grep real 
        ./DatasetP1/correttore DatasetP1/input/input$i.txt DatasetP1/helper/helper$i.txt output.txt
done

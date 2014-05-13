#!/bin/bash

tests=19
if [[ -n $1 ]]; then
    tests=$1
fi
range=0..`echo $tests`

for i in $(eval echo {$range})
do
        cp DatasetP2/input/input$i.txt input.txt
        echo -n "$i: "
        (time ./prescelto) &> /tmp/res && cat /tmp/res | grep real 
        ./DatasetP2/correttore DatasetP2/input/input$i.txt DatasetP2/helper/helper$i.txt output.txt
done

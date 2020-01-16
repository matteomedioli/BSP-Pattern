#!/bin/bash                                                                                         
cd ./data && rm *
cd ../                                                                                               
for par_deg in 1 4 8 16 32 64 128
do
    for n in 512000 1024000 2048000 4096000 8192000
    do
        if [ "$1" = "--verbose" ];
            then
                echo ""
                ./main $n $par_deg 1
            else
                echo "EXEC main with " $n "element " $par_deg " parallel activities"
                ./main $n $par_deg 0 >> test/results.csv
        fi
        
    done
done
exit 0

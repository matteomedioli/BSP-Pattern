#!/bin/bash                                                                                         \
                                                                                                     

# Declare an array of parallel activities NW                                                        \
                                                                                                     
declare -a par_deg=()
declare -a n=("512000" "1024000" "2048000" "4096000" "8192000")

# Iterate the string array using for loop                                                           \
                                                                                                     
#!/bin/bash                                                                                         \
                                                                                                     
for par_deg in 1 4 8 16 32 64 128
do
    for n in 512000 1024000 2048000 4096000 8192000
    do
        echo "EXEC main with " $n "element " $par_deg " parallel activities"
        ./main $n $par_deg >> ./test/results.csv
    done
done
exit 0

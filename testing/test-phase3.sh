test_files=( "./sem_simple.x" "./edge_test.x" "./sem_prime.x" "./sem_buffer.x" "./err_test.x" "./err_test2.x")
exp_output=( "exp_sem_simple.txt" "exp_sem_edge.txt" "exp_sem_prime.txt" "exp_sem_buffer.txt" "exp_sem_err.txt" "exp_sem_err2.txt")

cd ../libuthread 
make > /dev/null
cd ../apps
make > /dev/null
for ((i = 0; i < ${#test_files[@]}; i++)); do
    test_file="${test_files[$i]}"
    exp_output="${exp_output[$i]}"
    echo "Testing on: " $test_file $exp_output
	cd ../apps
    $test_file > ../testing/given.txt
    cd ../testing
    DIFF=$(diff -w -b $exp_output given.txt)
    if [ "$DIFF" != "" ]
    then
        echo "Fails: $DIFF"
    else
        echo "Passes"
    fi
done

rm given.txt
# cd ../libuthread
# make
# cd ../apps
# make
# ./sem_simple.x > ../testing/given.txt
# cd ../testing
# DIFF=$(diff -w -b exp_sem_count.txt given_sem_count.txt)
# if [ "$DIFF" != "" ]
# then
#     echo "Fails: $DIFF"
# else
#     echo "Passes"
# fi

# rm given_sem_count.txt


#valgrind --leak-check=yes --track-origins=yes ./sem_simple.x
test_files=( "./test_preempt.x" "./test_preempt_complex.x" )
exp_output=( "exp_test_preempt.txt" "exp_test_preempt_complex.txt" )

cd ../libuthread 
make &> /dev/null
cd ../apps
make &> /dev/null
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
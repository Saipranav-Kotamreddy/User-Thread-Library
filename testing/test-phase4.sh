test_files=( "./test_preempt.x" "./test_preempt_complex.x" "./err_test3.x" "./err_test4.x" "./test_preempt2.x")
exp_output=( "exp_test_preempt.txt" "exp_test_preempt_complex.txt" "exp_test_err_test3.txt" "exp_test_err_test4.txt" "exp_test_preempt2.txt" )

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
#test1
./myOS shell ./tests/test1.txt &
ps -f > ./tests/processes.txt
if grep -q "./myOS shell ./tests/test1.txt" "./tests/processes.txt" && grep -q "sleep 1" "./tests/processes.txt" ; then echo "Test 1 Succeeded" >> report.txt; else echo "Test 1 Failed" >> report.txt; fi
rm ./tests/processes.txt

#test2
./myOS shell ./tests/test2.txt
cat ./tests/Tyrion/1.txt | sort > ./tests/answer.txt
if diff ./tests/result.txt ./tests/answer.txt -q ; then echo "Test 2 Succeeded" >> report.txt; else echo "Test 2 Failed" >> report.txt; fi
rm ./tests/result.txt ./tests/answer.txt

# test3
./myOS shell ./tests/test3.txt
cat ./tests/Tyrion/2.txt | sort > ./tests/answer.txt
if diff ./tests/result.txt ./tests/answer.txt -q ; then echo "Test 3 Succeeded" >> report.txt; else echo "Test 3 Failed" >> report.txt; fi
rm ./tests/result.txt ./tests/answer.txt
HEAD = Test/TestUtility.h   src/Price4.h   src/ErrorMessage.h
SRC  = Test/TestUtility.cpp src/Price4.cpp src/ErrorMessage.cpp
FLAGS = --std=c++11

test-error_msg: Test/TestErrorMsg.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestErrorMsg.cpp $(SRC) -o test1
	./test1
	rm test1 error_msg

test-price4: Test/TestPrice4.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestPrice4.cpp $(SRC) -o test1
	./test1
	rm test1 error_msg

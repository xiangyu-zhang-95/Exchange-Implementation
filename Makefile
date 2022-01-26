test-error_msg: Test/TestErrorMsg.cpp src/ErrorMessage.h Test/TestUtility.cpp
	g++ --std=c++11 Test/TestErrorMsg.cpp Test/TestUtility.cpp -o test1
	./test1
	rm test1 error_msg
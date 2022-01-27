HEAD = src/OrderFlow.h   Test/TestUtility.h   src/Price4.h   src/ErrorMessage.h   src/TickerSize.h   src/Order.h
SRC  = src/OrderFlow.cpp Test/TestUtility.cpp src/Price4.cpp src/ErrorMessage.cpp src/TickerSize.cpp src/Order.cpp
FLAGS = --std=c++11

test-error_msg: Test/TestErrorMsg.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestErrorMsg.cpp $(SRC) -o test1
	./test1
	rm test1 error_msg

test-price4: Test/TestPrice4.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestPrice4.cpp $(SRC) -o test1
	./test1
	rm test1 error_msg

test-ticker_size: Test/TestTickerSize.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestTickerSize.cpp $(SRC) -o test1
	./test1
	rm test1 TestTickerSize_output

test-order: Test/TestOrder.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestOrder.cpp $(SRC) -o test1
	./test1
	rm test1 TestOrderOutput

test-order_flow: Test/TestOrderFlow.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestOrderFlow.cpp $(SRC) -o test1
	./test1
	rm test1 TestOrderFlowOutput

test-all:
	make test-error_msg
	make test-price4
	make test-ticker_size
	make test-order
	make test-order_flow

clean:
	rm test1 TestOrderOutput TestTickerSize_output error_msg TestOrderFlowOutput
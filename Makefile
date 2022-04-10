HEAD = src/OrderQueue.h   src/Feed.h   src/OrderBook.h   src/OrderFlow.h   Test/TestUtility.h   src/Price4.h   src/ErrorMessage.h   src/TickerSize.h   src/Order.h
SRC  = src/OrderQueue.cpp src/Feed.cpp src/OrderBook.cpp src/OrderFlow.cpp Test/TestUtility.cpp src/Price4.cpp src/ErrorMessage.cpp src/TickerSize.cpp src/Order.cpp
FLAGS = --std=c++11

test-error_msg: Test/TestErrorMsg.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestErrorMsg.cpp $(SRC) -o test1
	./test1
	rm -f test1 error_msg

test-price4: Test/TestPrice4.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestPrice4.cpp $(SRC) -o test1
	./test1
	rm -f test1 error_msg

test-ticker_size: Test/TestTickerSize.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestTickerSize.cpp $(SRC) -o test1
	./test1
	rm -f test1 TestTickerSize_output

test-order: Test/TestOrder.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestOrder.cpp $(SRC) -o test1
	./test1
	rm -f test1 TestOrderOutput

test-order_flow: Test/TestOrderFlow.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestOrderFlow.cpp $(SRC) -o test1
	./test1
	rm -f test1 TestOrderFlowOutput

test-feed: Test/TestFeed.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestFeed.cpp $(SRC) -o test1
	./test1
	rm -f test1 TestFeedOutput

test-order_book: Test/TestOrderBook.cpp $(SRC) $(HEAD)
	g++ $(FLAGS) Test/TestOrderBook.cpp $(SRC) -o test1
	./test1
	rm -f test1 TestOrderBook_output1 TestOrderBook_output2

test-all:
	make test-error_msg
	make test-price4
	make test-ticker_size
	make test-order
	make test-order_flow
	make test-feed
	make test-order_book

clean:
	rm -f TestOrderBook_output1 TestFeedOutput test1 TestOrderOutput 
	rm -f TestTickerSize_output error_msg TestOrderFlowOutput

order_book:
	g++ $(FLAGS) main.cpp src/OrderQueue.cpp src/Feed.cpp src/OrderBook.cpp src/Price4.cpp src/ErrorMessage.cpp src/Order.cpp -o test1
	./test1

syntax:
	g++ $(FLAGS) syntax.cpp -o syntax
	./syntax
	rm -f syntax
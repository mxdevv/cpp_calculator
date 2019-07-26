CC=g++
MAIN=main
TEST=gtest

all:
	$(CC) $(MAIN).cpp -o $(MAIN) -std=c++17

test:
	$(CC) $(TEST).cpp -o $(TEST) -lgtest -std=c++17

clean:
	rm $(MAIN) $(TEST)

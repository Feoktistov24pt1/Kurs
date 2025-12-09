CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

COMMON_SOURCES = Interface.cpp DataFileRead.cpp Authentication.cpp

TEST_SOURCES = tests.cpp $(COMMON_SOURCES)
TEST_TARGET = tests
TEST_CXXFLAGS = $(CXXFLAGS) -I/usr/include/UnitTest++
TEST_LIBS = -lcrypto -lUnitTest++

$(TEST_TARGET): $(TEST_SOURCES)
	$(CXX) $(TEST_CXXFLAGS) -o $@ $^ $(TEST_LIBS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

CLIENT_SOURCES = main.cpp $(COMMON_SOURCES) DataClient.cpp
CLIENT_TARGET = client
CLIENT_LIBS = -lcrypto

$(CLIENT_TARGET): $(CLIENT_SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CLIENT_LIBS)

.PHONY: all test clean

all: $(CLIENT_TARGET) $(TEST_TARGET)

clean:
	rm -f $(CLIENT_TARGET) $(TEST_TARGET)

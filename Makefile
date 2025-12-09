CXX = g++

COMMON_CXXFLAGS = -std=c++17 -O2 -Wall
COMMON_LIBS = -lcrypto

MAIN_SOURCES = main.cpp Interface.cpp DataFileRead.cpp Authentication.cpp DataClient.cpp
MAIN_TARGET = client

TEST_SOURCES = tests.cpp Interface.cpp DataFileRead.cpp Authentication.cpp
TEST_TARGET = tests
TEST_CXXFLAGS = $(COMMON_CXXFLAGS) -I/usr/include/UnitTest++
TEST_LIBS = $(COMMON_LIBS) -lUnitTest++

.PHONY: client test clean

client: $(MAIN_TARGET)

$(MAIN_TARGET): $(MAIN_SOURCES)
	$(CXX) $(COMMON_CXXFLAGS) -o $@ $^ $(COMMON_LIBS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SOURCES)
	$(CXX) $(TEST_CXXFLAGS) -o $@ $^ $(TEST_LIBS)

clean:
	rm -f $(MAIN_TARGET) $(TEST_TARGET)

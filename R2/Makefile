CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -I/usr/include/UnitTest++

SOURCES = tests.cpp Interface.cpp DataFileRead.cpp Authentication.cpp

TARGET = tests
LIBS = -lcrypto -lUnitTest++

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)
	
test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: test clean

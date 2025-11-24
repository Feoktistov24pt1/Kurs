CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
LDLIBS = -lcrypto

TARGET = client
SOURCES = main.cpp Interface.cpp DataFileRead.cpp Authentication.cpp DataClient.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(TARGET)

.PHONY: clean

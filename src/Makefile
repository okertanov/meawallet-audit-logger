CXXFLAGS:=-std=c++17 -Wall -Wextra -I./
LDFLAGS:=
LDLIBS:=

all: build

build: build-app build-tests

build-app: audit-logger

audit-logger: audit-logger.cpp syslog/syslog.cpp
	${CXX} ${CXXFLAGS} $? ${LDFLAGS} ${LDLIBS} -o $@

build-tests:

test:
	
clean:
	-@rm -f *.o
	-@rm -f *.a
	-@rm -f audit-logger


.PHONY: all build build-app build-tests test clean

.SILENT: clean

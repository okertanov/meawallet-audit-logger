#CXX:=g++
CXX:=clang++
CXXFLAGS:=-std=c++17 -Wall -Wextra -I./
LDFLAGS:=
LDLIBS:=

all: build

build: build-app build-tests

build-app: audit-logger

audit-logger: audit-logger.cpp syslog/syslog.cpp
	${CXX} ${CXXFLAGS} $? ${LDFLAGS} ${LDLIBS} -o $@

build-tests: tests/app-test tests/crypto-test tests/audit-test

tests/app-test: tests/app-test.cpp syslog/syslog.cpp
	${CXX} ${CXXFLAGS} $? ${LDFLAGS} ${LDLIBS} -o $@

tests/crypto-test: tests/crypto-test.cpp syslog/syslog.cpp
	${CXX} ${CXXFLAGS} $? ${LDFLAGS} ${LDLIBS} -o $@

tests/audit-test: tests/audit-test.cpp syslog/syslog.cpp
	${CXX} ${CXXFLAGS} $? ${LDFLAGS} ${LDLIBS} -o $@

tests/storage-test: tests/storage-test.cpp syslog/syslog.cpp
	${CXX} ${CXXFLAGS} $? ${LDFLAGS} ${LDLIBS} -o $@

tests: tests/app-test tests/crypto-test tests/audit-test tests/storage-test
	cd tests && ./app-test
	cd tests && ./crypto-test
	cd tests && ./audit-test
	cd tests && ./storage-test
	
clean:
	-@rm -f *.o
	-@rm -f *.a
	-@rm -f audit-logger
	-@rm -f tests/app-test tests/crypto-test tests/audit-test tests/storage-test
	-@rm -rf tests/storage


.PHONY: all build build-app build-tests tests clean

.SILENT: clean

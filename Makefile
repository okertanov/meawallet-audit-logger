all: build

build: build-app build-tests

build-app:
	make -C src $@
	cp src/audit-logger ./

build-tests:
	make -C src $@

test:
	make -C src $@

audit-logger: build-app

run-dump: audit-logger
	./audit-logger

run-count: audit-logger
	./audit-logger --count

run-reset: audit-logger
	./audit-logger --reset

run-monitor: audit-logger
	./audit-logger --monitor

run-log: audit-logger
	./audit-logger --data="${DATA}"

clean:
	make -C src $@
	-@rm -f audit-logger

sys-install:

.PHONY: all build build-app build-tests test run-dump run-count run-reset run-monitor run-log clean sys-install

.SILENT: clean

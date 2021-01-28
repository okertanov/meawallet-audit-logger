all: build

build: build-app build-tests

build-app:
	make -C src $@
	cp src/audit-logger ./

build-tests:
	make -C src $@

tests:
	make -C src $@

audit-logger: build-app

run-help: audit-logger
	./audit-logger --help

run-dump: audit-logger
	./audit-logger

run-count: audit-logger
	./audit-logger --count

run-reset: audit-logger
	./audit-logger --reset

run-monitor: audit-logger
	./audit-logger --monitor

run-log: audit-logger
	./audit-logger --entry="${ENTRY}"

clean:
	make -C src $@
	-@rm -f audit-logger

sys-install:

.PHONY: all build build-app build-tests tests run-dump run-count run-reset run-monitor run-log clean sys-install

.SILENT: clean

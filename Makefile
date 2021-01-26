all: build

build: build-app build-tests

build-app:
	make -C src $@
	cp src/audit-logger ./

build-tests:
	make -C src $@

test:
	make -C src $@

run-dump:
	audit-logger

run-count:
	audit-logger --count

run-reset:
	audit-logger --reset

run-monitor:
	audit-logger --monitor

run-log:
	audit-logger --data="${DATA}"

clean:
	make -C src $@
	-@rm -f audit-logger

sys-install:

.PHONY: all build build-app build-tests test run-dump run-count run-reset run-monitor run-log clean sys-install

.SILENT: clean

MeaWallet C++ Developer’s Home Task
===================================

Architecture overview
---------------------

    +---------------------+ +--------------------+        +---------------+
    |    System Logger    | |                    +------->+  Commandline  |
    +---------------------+ |   Configuration    |        +---------------+
    |                     | |                    |        +---------------+
    |   Console logger    | |                    +------->+  Environment  |
    +-------+-------------+ +---------+----------+        +---------------+
            ^                       ^
            |                       |
    +-------+-----------------------+-------------------------------------+
    | Application                                                         |
    |                                                                     |
    | +----------------+          +----------------+                      |
    | | Audit logger   +--+----+->+Secure storage  +<-------------+       |
    | |                |  |    ^  |                |              |       |
    | +----------------+  |    |  +----------------+------------+ |       |
    |                     |    |                                | |       |
    |                     |    |                                | |       |
    |                     v    |                 +--------------v-+--+    |
    |               +-----+----+-------+         | Crypto module     |    |
    |               |Log interceptors  +-------->+                   |    |
    |               |                  |         | +---------------+ |    |
    |               +------------------+<--------+ |     AES|CBC   | |    |
    |                                            +-+---------------+-+    |
    |                                                                     |
    +---------------------------------------------------------------------+


Requirements
------------
    
    - Gnu Make
    - Linux Build essentials (inluding GCC/G++)
    - OpenSSL (including dev libs)


Pre-install base requirements
-----------------------------

    sudo make sys-install


Build, test and run
-------------------

    make clean all
    make tests

    make run-help
    make run-log LOG="{....}"
    make run-count
    make run-dump
    make run-reset

    make clean


Data format
-----------

    {
        {
            created_at: 2020-12-01 12:00:00
            user_id: Employee_1
            event_type: 3
            additional_data: "User logged in."
            error_codes: "678|159|262"
        },
        {
            created_at: 2020-12-01 12:01:00
            user_id: Employee_1
            event_type: 1
            additional_data: "User logged out."
            error_codes: null
        },
        {
            created_at: 2020-12-01 12:05:00
            user_id: null
            event_type: 5
            additional_data: "App data wiped."
            error_codes: null
        }
    }
    ...


Misc
----
### MacOS

    brew install openssl
    brew link openssl


TODO
----
    new audit with logger with output new console or file writer with interceptor with crypto
    new app with config with cmdline with audit

    --help
    --datadir=storage
    --debug
    --monitor
    --count
    --reset

    --datadir=storage
    --debug
    --entry="{...}"


Issues
------
    - No JSON parser/validator;
    - No sequential date/time check for entries;
    - Commandline params handling isn't restrictive and accept anything;

Links
-----
[https://asecuritysite.com/encryption/ccmaes](https://asecuritysite.com/encryption/ccmaes)  
[https://www.openssl.org/docs/manmaster/man1/openssl-mac.html](https://www.openssl.org/docs/manmaster/man1/openssl-mac.html)  
[https://stackoverflow.com/questions/33017333/different-encryption-results-between-platforms-using-openssl](https://stackoverflow.com/questions/33017333/different-encryption-results-between-platforms-using-openssl)  
[http://rubenlaguna.com/post/2015-02-05-compute-aes-cmac-using-openssl-slash-libcrypto/](http://rubenlaguna.com/post/2015-02-05-compute-aes-cmac-using-openssl-slash-libcrypto/)  

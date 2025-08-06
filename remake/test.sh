#!/bin/bash
set -e

gcc -o test_dispatch \
    main.c \
    dispatch.c \
    bulitin_func.c \
    returns.c \
    -lpthread -ldl -lssl -lcrypto

./testment

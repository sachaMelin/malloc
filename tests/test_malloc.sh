#!/bin/sh

test_malloc() {
    $1 > expected
    LD_PRELOAD=./libmalloc.so $1 > actual
    diff --color -c actual expected 
}

test_malloc "factor 20 30 40 50 60 70 80 90"
test_malloc "cat Makefile"
test_malloc "ls"
test_malloc "ip a"

rm expected actual

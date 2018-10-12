#!/usr/bin/env bash
./echo_arg csc209 > echo_out.txt
cat echo_stdin.c | ./echo_stdin
./count 210 | wc -m
ls -S | head -1 | ./echo_stdin

#!/bin/bash

cd test-server
gcc CuTest.c test-server.c -o s
cd ..

cd client
gcc client.c -o c
cd ..

#!/bin/bash

mpirun --oversubscribe -np 10 ./main "$@"

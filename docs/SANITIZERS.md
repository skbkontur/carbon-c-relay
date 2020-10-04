# valgrind

Run tests under valgrind
```
export VALGRIND=1
make test 2>&1 | tee valgrind.log
```

run build-test.sh with tests under valgrind
```
VALGRIND=1 ./build-test.sh 2>&1 | tee valgrind.log
```

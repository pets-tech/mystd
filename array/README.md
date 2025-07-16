```bash
cmake -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake ..
make

./unit_tests or ctest
./benchmark_tests
./main
```
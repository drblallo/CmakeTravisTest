#!/bin/bash
cd cmake-build-coverage/
cmake .. -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage"
rm test.info
rm filtered.info
rm base.info
rm total.info
lcov --capture --initial --directory . --output-file ./base.info
cmake --build . --target all
cmake --build . --target test
lcov --capture --directory . --output-file ./test.info
lcov --add-tracefile ./base.info --add-tracefile ./test.info --output-file ./total.info
lcov --remove ./total.info '/usr/*' '*/googletest/*' --output-file ./filtered.info
genhtml ./filtered.info --output-directory ./Coverage

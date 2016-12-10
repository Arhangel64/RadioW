cmake_minimum_required(VERSION 2.8.12)

add_jslib(wTest/test.js lib/wTest/test ${MAGNUS_DIR} node)
add_jslib(wTest/abstractmap.js lib/wTest/abstractmap ${MAGNUS_DIR} node)
add_jslib(wTest/abstractlist.js lib/wTest/abstractlist ${MAGNUS_DIR} node)
add_jslib(wTest/abstractorder.js lib/wTest/abstractorder ${MAGNUS_DIR} node)
add_jslib(wTest/uint64.js lib/wTest/uint64 ${MAGNUS_DIR} node)

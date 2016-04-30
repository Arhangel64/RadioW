cmake_minimum_required(VERSION 2.8.12)

add_jslib(wTest/test.js lib/wTest/test ${LORGAR_DIR} browser)
add_jslib(wTest/abstractmap.js lib/wTest/abstractmap ${LORGAR_DIR} browser)
add_jslib(wTest/abstractlist.js lib/wTest/abstractlist ${LORGAR_DIR} browser)
add_jslib(wTest/abstractorder.js lib/wTest/abstractorder ${LORGAR_DIR} browser)
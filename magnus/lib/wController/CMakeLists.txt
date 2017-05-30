cmake_minimum_required(VERSION 2.8.12)

add_jslib(wController/list.js lib/wController/list ${MAGNUS_DIR} node)
add_jslib(wController/controller.js lib/wController/controller ${MAGNUS_DIR} node)
add_jslib(wController/vocabulary.js lib/wController/vocabulary ${MAGNUS_DIR} node)

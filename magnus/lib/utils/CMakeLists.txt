cmake_minimum_required(VERSION 2.8.12)

add_jslib(utils/class.js lib/utils/class ${MAGNUS_DIR} node)
add_jslib(utils/subscribable.js lib/utils/subscribable ${MAGNUS_DIR} node)
add_jslib(utils/globalMethods.js lib/utils/globalMethods ${MAGNUS_DIR} node)
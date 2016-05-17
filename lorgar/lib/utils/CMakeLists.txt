cmake_minimum_required(VERSION 2.8.12)

add_jslib(utils/class.js lib/utils/class ${LORGAR_DIR} browser)
add_jslib(utils/subscribable.js lib/utils/subscribable ${LORGAR_DIR} browser)
add_jslib(utils/globalMethods.js lib/utils/globalMethods ${LORGAR_DIR} browser)
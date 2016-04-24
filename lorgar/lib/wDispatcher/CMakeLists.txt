cmake_minimum_required(VERSION 2.8.12)

add_jslib(wDispatcher/dispatcher.js lib/wDispatcher/dispatcher ${LORGAR_DIR} browser)
add_jslib(wDispatcher/defaulthandler.js lib/wDispatcher/defaulthandler ${LORGAR_DIR} browser)
add_jslib(wDispatcher/handler.js lib/wDispatcher/handler ${LORGAR_DIR} browser)
add_jslib(wDispatcher/logger.js lib/wDispatcher/logger ${LORGAR_DIR} browser)
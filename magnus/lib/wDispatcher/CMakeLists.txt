cmake_minimum_required(VERSION 2.8.12)

add_jslib(wDispatcher/dispatcher.js lib/wDispatcher/dispatcher ${MAGNUS_DIR} node)
add_jslib(wDispatcher/defaulthandler.js lib/wDispatcher/defaulthandler ${MAGNUS_DIR} node)
add_jslib(wDispatcher/handler.js lib/wDispatcher/handler ${MAGNUS_DIR} node)
add_jslib(wDispatcher/logger.js lib/wDispatcher/logger ${MAGNUS_DIR} node)
add_jslib(wDispatcher/parentreporter.js lib/wDispatcher/parentreporter ${MAGNUS_DIR} node)

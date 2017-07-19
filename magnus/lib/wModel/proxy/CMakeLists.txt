cmake_minimum_required(VERSION 2.8.12)

add_jslib(wModel/proxy/proxy.js lib/wModel/proxy/proxy ${MAGNUS_DIR} node)
add_jslib(wModel/proxy/list.js lib/wModel/proxy/list ${MAGNUS_DIR} node)
add_jslib(wModel/proxy/vocabulary.js lib/wModel/proxy/vocabulary ${MAGNUS_DIR} node)
add_jslib(wModel/proxy/catalogue.js lib/wModel/proxy/catalogue ${MAGNUS_DIR} node)

configure_file(pane.js pane.js)

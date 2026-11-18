# XMPP backend for gram (Telegram Desktop UI on top of XMPP).
# QXmpp is built separately (Telegram/build/xmpp/build_qxmpp.sh) and found
# through CMAKE_PREFIX_PATH.

find_package(QXmppQt6 REQUIRED)

add_library(td_xmpp OBJECT)
init_non_host_target(td_xmpp)
add_library(tdesktop::td_xmpp ALIAS td_xmpp)

nice_target_sources(td_xmpp ${src_loc}
PRIVATE
    xmpp/xmpp_client.cpp
    xmpp/xmpp_client.h
)

target_include_directories(td_xmpp
PUBLIC
    ${src_loc}
)

target_link_libraries(td_xmpp
PUBLIC
    desktop-app::lib_base
    QXmpp::QXmpp
)

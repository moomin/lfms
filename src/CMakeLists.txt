cmake_minimum_required(VERSION 2.8)

add_library(lfmsl Lfms.cpp LfmsConfig.cpp LfmsSession.cpp LfmsTrack.cpp LfmsWsApi.cpp)

find_package(LibXml2)
find_package(OpenSSL)

include_directories(${LIBXML2_INCLUDE_DIR})

add_library(helpers Log.cpp XmlParser.cpp helpers.cpp)

include_directories(${lfms_SOURCE_DIR/src})

add_executable(lfms main.cpp)

target_link_libraries(lfms lfmsl helpers ${LIBXML2_LIBRARIES}i ${OPENSSL_LIBRARIES})


cmake_minimum_required(VERSION 2.8)

add_library(lfmsl Lfms.cpp LfmsConfig.cpp LfmsSession.cpp LfmsTrack.cpp LfmsWsApi.cpp)
add_library(helpers HttpClient.cpp Log.cpp XmlParser.cpp helpers.cpp)

find_package(LibXml2)
find_package(OpenSSL)

include_directories(${LIBXML2_INCLUDE_DIR})

add_executable(lfms main.cpp)

target_link_libraries(lfms lfmsl helpers ${LIBXML2_LIBRARIES} ${OPENSSL_LIBRARIES})


set(PLUG_NAME clixon-plug-system)

set(FMT_LIB ${CMAKE_CURRENT_SOURCE_DIR}/libfmt.a)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(PLUG_SOURCES
        main.cpp
)

add_library(${PLUG_NAME} SHARED ${PLUG_SOURCES})

#get_version(version.h)
#
#set_target_properties(${PLUG_NAME} PROPERTIES SOVERSION ${VERSION_API})
#set_target_properties(${PLUG_NAME} PROPERTIES VERSION ${VERSION_API}.${VERSION_APP})

target_link_libraries(${PLUG_NAME} PRIVATE ${FMT_LIB} clixon_cli)


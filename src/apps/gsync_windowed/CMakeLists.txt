set(TARGET_NAME gsync_windowed)

set(${TARGET_NAME}_HEADERS
)

set(${TARGET_NAME}_SOURCES
    main.cpp
)

add_executable(${TARGET_NAME}
               ${${TARGET_NAME}_HEADERS}
               ${${TARGET_NAME}_SOURCES}
)

target_link_libraries(${TARGET_NAME}
)

set_target_properties(${TARGET_NAME} PROPERTIES
                      VERSION "${${PROJECT_NAME}_VERSION}"
)

install(TARGETS ${TARGET_NAME})

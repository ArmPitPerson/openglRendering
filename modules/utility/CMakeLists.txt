# Module Name
SET(MODULE_NAME libutility)
SET(MODULE_EXPORT_NAME ${MODULE_NAME}Targets)

# Create library and alias for building as a sub-project
ADD_LIBRARY(${MODULE_NAME} STATIC "")
ADD_LIBRARY(${MODULE_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})

# Add include directories
TARGET_INCLUDE_DIRECTORIES(${MODULE_NAME}
                           PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>
                           PRIVATE
                           "${CMAKE_CURRENT_SOURCE_DIR}"
                           "${CMAKE_SOURCE_DIR}/ext/spdlog/include"
                           )

# Add source files
TARGET_SOURCES(${MODULE_NAME}
               PRIVATE
               ${CMAKE_CURRENT_SOURCE_DIR}/include/clock.h
               ${CMAKE_CURRENT_SOURCE_DIR}/include/files.h
               ${CMAKE_CURRENT_SOURCE_DIR}/include/logging.h
               ${CMAKE_CURRENT_SOURCE_DIR}/include/randomEngine.h
               ${CMAKE_CURRENT_SOURCE_DIR}/src/clock.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/src/files.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/src/logging.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/src/randomEngine.cpp
               )

# Dependencies
TARGET_LINK_LIBRARIES(${MODULE_NAME} PCG::PCG)

# Set Install Targets
INSTALL(TARGETS ${MODULE_NAME} EXPORT ${MODULE_EXPORT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

INSTALL(EXPORT ${MODULE_EXPORT_NAME}
        FILE ${MODULE_EXPORT_NAME}.cmake
        NAMESPACE ${MODULE_NAME}::
        DESTINATION lib/cmake/${MODULE_NAME}
        )

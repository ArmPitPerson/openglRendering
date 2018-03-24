# Module Name
SET(MODULE_NAME libcomputation)
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
                           )

# Add source files
TARGET_SOURCES(${MODULE_NAME}
               PRIVATE
               ${CMAKE_CURRENT_SOURCE_DIR}/include/interpolation.h
               ${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/include/linalg.h
               )

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

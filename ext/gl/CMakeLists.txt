# Module Name
SET(MODULE_NAME glLoadGen)
SET(MODULE_EXPORT_NAME ${MODULE_NAME}Targets)

# Create library and alias for building as a sub-project
ADD_LIBRARY(${MODULE_NAME} STATIC "")
ADD_LIBRARY(${MODULE_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})

# Add include directories
TARGET_INCLUDE_DIRECTORIES(${MODULE_NAME}
                           PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>
                           )

TARGET_SOURCES(${MODULE_NAME}
               PRIVATE
               ${CMAKE_CURRENT_SOURCE_DIR}/include/gl_cpp.hpp
               ${CMAKE_CURRENT_SOURCE_DIR}/src/gl_cpp.cpp
               )

if(MSVC)
  TARGET_LINK_LIBRARIES(${MODULE_NAME} opengl32)
else()
  TARGET_LINK_LIBRARIES(${MODULE_NAME} OpenGL)
endif()

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

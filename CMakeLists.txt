project(SensorIntegration)


# source files
aux_source_directory(. SensorIntegration_SRCS)
INCLUDE_DIRECTORIES(.)

FILE(GLOB_RECURSE SensorIntegration_INCLUDE_LIST ./*.h ./*.hpp)



#upload include and link names
set(SUBDIR_INCLUDE_DIRECTORIES ${SUBDIR_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set(SUBDIR_LINK_NAMES ${SUBDIR_LINK_NAMES} ${PROJECT_NAME} PARENT_SCOPE)

#create the lib
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SensorIntegration_SRCS} ${SensorIntegration_INCLUDE_LIST})


find_package(Qt5Core REQUIRED)
qt5_use_modules(SensorIntegration Core SerialPort)
qt5_use_modules(SensorIntegration Widgets)


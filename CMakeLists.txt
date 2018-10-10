project(SensorIntegration)


# source files
aux_source_directory(. SensorIntegration_SRCS)
FILE(GLOB_RECURSE SensorIntegration_INCLUDE_LIST ./*.h ./*.hpp)


find_package(Qt5SerialPort REQUIRED)
#why the fuck this do not work?
#set(SUBDIR_INCLUDE_DIRECTORIES ${SUBDIR_INCLUDE_DIRECTORIES} ${Qt5SerialPort_INCLUDE_DIRS} PARENT_SCOPE)
#find_package(Qt5Core REQUIRED)
#set(SUBDIR_INCLUDE_DIRECTORIES ${SUBDIR_INCLUDE_DIRECTORIES} ${Qt5Core_INCLUDE_DIRS} PARENT_SCOPE)



#upload include and link names
set(SUBDIR_INCLUDE_DIRECTORIES ${SUBDIR_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} ${Qt5SerialPort_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} PARENT_SCOPE)
set(SUBDIR_LINK_NAMES ${SUBDIR_LINK_NAMES} ${PROJECT_NAME} PARENT_SCOPE)

#INCLUDE_DIRECTORIES(SUBDIR_INCLUDE_DIRECTORIES)

#create the lib
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SensorIntegration_SRCS} ${SensorIntegration_INCLUDE_LIST})

#target_link_libraries(${PROJECT_NAME} Qt5SerialPort_LIBRARIES Qt5Core_LIBRARIES)
qt5_use_modules(${PROJECT_NAME} Core SerialPort)
qt5_use_modules(${PROJECT_NAME} Widgets)



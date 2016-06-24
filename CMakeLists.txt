project(SensorIntegration)


# source files
aux_source_directory(. SensorIntegration_SRCS)
#INCLUDE_DIRECTORIES(.)

FILE(GLOB_RECURSE SensorIntegration_INCLUDE_LIST ./*.h ./*.hpp)


ADD_LIBRARY(${PROJECT_NAME} STATIC ${SensorIntegration_SRCS} ${SensorIntegration_INCLUDE_LIST})





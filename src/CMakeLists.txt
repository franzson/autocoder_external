include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-pretarget.cmake)

#############################################################
# MAX EXTERNAL
#############################################################

include_directories(
    "${MAX_SDK_INCLUDES}"
    "${MAX_SDK_MSP_INCLUDES}"
    "${MAX_SDK_JIT_INCLUDES}"
    "~/Downloads/tensorflow-master/tensorflow/lite/c"
)

file(GLOB PROJECT_SRC
     "*.h"
     "*.c"
     "*.cpp"
)
add_library(
    ${PROJECT_NAME}
    MODULE
    ${PROJECT_SRC}
    "~/Downloads/tensorflow-master/tensorflow/lite/c"
    "/usr/local/lib"
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-posttarget.cmake)

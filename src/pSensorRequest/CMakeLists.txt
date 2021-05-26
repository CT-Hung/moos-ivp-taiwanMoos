#--------------------------------------------------------
# The CMakeLists.txt for:                       pSensorRequest
# Author(s):                              Chao-Chun Hsu
#--------------------------------------------------------

SET(SRC
  SensorRequest.cpp
  SensorRequest_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pSensorRequest ${SRC})

TARGET_LINK_LIBRARIES(pSensorRequest
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)
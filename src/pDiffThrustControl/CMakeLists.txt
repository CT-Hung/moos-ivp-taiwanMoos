#--------------------------------------------------------
# The CMakeLists.txt for:                 pDiffThrustControl
# Author(s):                              Chao-Chun Hsu
#--------------------------------------------------------

SET(SRC
  DiffThrustControl.cpp
  DiffThrustControl_Info.cpp
  main.cpp
  newM200.cpp
  pid.cpp
)

ADD_EXECUTABLE(pDiffThrustControl ${SRC})

TARGET_LINK_LIBRARIES(pDiffThrustControl
   ${MOOS_LIBRARIES}
   geometry
   apputil
   mbutil
   m
   pthread)
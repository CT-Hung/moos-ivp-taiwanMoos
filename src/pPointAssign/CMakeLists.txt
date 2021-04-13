#--------------------------------------------------------
# The CMakeLists.txt for:                       pPointAssign
# Author(s):                              Chao-Chun Hsu
#--------------------------------------------------------

SET(SRC
  Point.cpp
  PointAssign.cpp
  PointAssign_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pPointAssign ${SRC})

TARGET_LINK_LIBRARIES(pPointAssign
   ${MOOS_LIBRARIES}
   geometry
   apputil
   mbutil
   m
   pthread)


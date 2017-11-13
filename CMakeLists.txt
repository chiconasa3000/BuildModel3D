cmake_minimum_required(VERSION 2.8)

PROJECT(Create3DBones)

#Habilita c++
if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

#Archivos DELAUNAY_UTIL
set(D_DIR "/home/cris/ExtraPrograms/QtProjects/Example/Example/DelaunayUtil")
set(D_DIRB "/home/cris/ExtraPrograms/QtProjects/Example/Example")
set(DELAUNAY_CPP ${D_DIR}/arista.cpp ${D_DIR}/grafo.cpp ${D_DIR}/subgrafo.cpp ${D_DIR}/punto.cpp ${D_DIR}/utilmaths.cpp)

#set(DELAUNAY_HEAD ${D_DIR}/arista.h ${D_DIR}/grafo.h ${D_DIR}/subgrafo.h ${D_DIR}/.h ${D_DIR}/punto.h)
include_directories( ${D_DIR} )

#Archivos ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#Archivos VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#Archivos EIGEN
find_package(Eigen3 REQUIRED)
include_directories(EIGEN3_INCLUDE_DIR)

#Librerias QT
if(${VTK_VERSION} VERSION_GREATER "6" )
  # Instruct CMake to run moc automatically when needed.
  set(CMAKE_AUTOMOC ON) #asigna los QOBJECT como parte del proyecto
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
  ADD_DEFINITIONS(${QT_DEFINITIONS})
else()
endif()

#Visor de archivos
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB UI_FILES *.ui)
file(GLOB QT_WRAP *.h)
file(GLOB CXX_FILES *.cxx)

#Generacion de ejecutable encapsulando QT
if(${VTK_VERSION} VERSION_GREATER "6")
  qt4_wrap_ui(UISrcs ${UI_FILES} )
  # CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
  add_executable(Create3DBones MACOSX_BUNDLE  ${DELAUNAY_CPP} ${CXX_FILES} ${UISrcs} ${QT_WRAP} ${OBJ_FILES})
  qt4_use_modules(Create3DBones Core Gui)
  target_link_libraries(Create3DBones ${VTK_LIBRARIES} ${ITK_LIBRARIES})
endif()

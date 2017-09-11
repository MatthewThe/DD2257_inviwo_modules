#--------------------------------------------------------------------
# Inviwo DD2257Lab1 Module
ivw_module(DD2257Lab1)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/generate2ddata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parallelcoordinates.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scatterplot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/plot2drenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/column.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/csvreader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/csvsource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/dataframe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/datapoint.h
)
#~ ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/generate2ddata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parallelcoordinates.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scatterplot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/plot2drenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/column.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/csvreader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/csvsource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/dataframe.cpp
)
ivw_group("Sources" ${SOURCE_FILES} ${HEADER_FILES})


#--------------------------------------------------------------------
# Add shaders
set(SHADER_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/glsl/dd2257lab1processor.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/plot2drenderer.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/plot2drenderer.vert
)
ivw_group("Shader Files" ${SHADER_FILES})


#--------------------------------------------------------------------
# Add Unittests
set(TEST_FILES
	#${CMAKE_CURRENT_SOURCE_DIR}/tests/unittests/dd2257lab1-test.cpp
)
ivw_add_unittest(${TEST_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
# ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)

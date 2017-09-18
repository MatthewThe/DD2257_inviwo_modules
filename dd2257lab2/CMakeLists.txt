#--------------------------------------------------------------------
# Inviwo DD2257Lab2 Module
ivw_module(DD2257Lab2)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/dd2257lab2processor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/marchingsquares.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/amirameshvolumereader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/setminmaxdatamap.h
)
#~ ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/dd2257lab2processor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/marchingsquares.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/amirameshvolumereader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/setminmaxdatamap.cpp
)
ivw_group("Sources" ${SOURCE_FILES} ${HEADER_FILES})


#--------------------------------------------------------------------
# Add shaders
set(SHADER_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/glsl/dd2257lab2processor.frag
)
ivw_group("Shader Files" ${SHADER_FILES})


#--------------------------------------------------------------------
# Add Unittests
set(TEST_FILES
	#${CMAKE_CURRENT_SOURCE_DIR}/tests/unittests/dd2257lab2-test.cpp
)
ivw_add_unittest(${TEST_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
# ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)

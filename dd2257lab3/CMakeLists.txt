#--------------------------------------------------------------------
# Inviwo DD2257Lab3 Module
ivw_module(DD2257Lab3)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/dd2257lab3processor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/integrator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/streamlineintegrator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/eulerrk4comparison.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/vectorfieldgenerator2d.h
)
#~ ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/dd2257lab3processor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/integrator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/streamlineintegrator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/eulerrk4comparison.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/vectorfieldgenerator2d.cpp
)
ivw_group("Sources" ${SOURCE_FILES} ${HEADER_FILES})


#--------------------------------------------------------------------
# Add shaders
set(SHADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/vectorfieldgenerator.frag
)
ivw_group("Shader Files" ${SHADER_FILES})


#--------------------------------------------------------------------
# Add Unittests
set(TEST_FILES
	#${CMAKE_CURRENT_SOURCE_DIR}/tests/unittests/dd2257lab3-test.cpp
)
ivw_add_unittest(${TEST_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
# ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)

include(${CMAKE_SOURCE_DIR}/cmake/pr_common.cmake)

option(MATHUTIL_ENABLE_MESH_FUNCTIONS "Enable mesh functions, requires geometric tools library." ON)
option(MATHUTIL_STATIC "Build as static library?" OFF)
option(MATHUTIL_BUILD_TESTS "Build tests of library?" OFF)
option(LINK_COMMON_LIBS_STATIC "Link to common Pragma libraries statically?" OFF)

if(${MATHUTIL_STATIC})
	set(LIB_TYPE STATIC)
else()
	set(LIB_TYPE SHARED)
endif()

set(PROJ_NAME mathutil)
pr_add_library(${PROJ_NAME} ${LIB_TYPE})
pr_add_export_macro(${PROJ_NAME} DLLMUTIL)

if (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	# WINDOWS_CLANG_COMPILER_FIX
	# Using PRM_ prefix temporarily to avoid naming conflicts with the macros
	target_compile_definitions(${PROJ_NAME} PUBLIC
		"PRM_UNIT=GET_UNIT()"

		"PRM_BIAS=GET_BIAS()"

		"PRM_FORWARD=GET_FORWARD()"
		"PRM_UP=GET_UP()"
		"PRM_RIGHT=GET_RIGHT()"
		"PRM_ORIGIN=GET_ORIGIN()"
		"IDENTITY_SCALE=GET_IDENTITY_SCALE()"
		"PRM_MIN=GET_MIN()"
		"PRM_MAX=GET_MAX()"
	)
endif()

set(IMPORT_GLM_AS_MODULE TRUE)
if(IMPORT_GLM_AS_MODULE)
	add_subdirectory("third_party_libs/glm_cxxmodule")
else()
	set(DEPENDENCY_GLM_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/third_party_libs/glm_cxxmodule/third_party_libs/glm/" CACHE PATH "Path to glm include directory." FORCE)
endif()

if(IMPORT_GLM_AS_MODULE)
	pr_add_dependency(${PROJ_NAME} glm_module TARGET PUBLIC)
endif()
pr_add_dependency(${PROJ_NAME} util_string TARGET)
if(NOT IMPORT_GLM_AS_MODULE)
	pr_add_external_dependency(${PROJ_NAME} glm HEADER_ONLY PUBLIC)
endif()
pr_add_external_dependency(${PROJ_NAME} geometric_tools HEADER_ONLY)

if(${MATHUTIL_BUILD_TESTS})
	pr_add_external_dependency(${PROJ_NAME} google_tests HEADER_ONLY)
endif()

pr_add_headers(${PROJ_NAME} "include/")
pr_init_module(${PROJ_NAME})

# magic_enum
target_include_directories(${PROJ_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third_party_libs/magic_enum/include/")
pr_add_module_list(${PROJ_NAME} cxx_modules_impl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/third_party_libs/magic_enum/module/magic_enum.cppm")

pr_add_compile_definitions(
	${PROJ_NAME}
		-DSHUTIL_STATIC
		-DMUTIL_DLL
)
pr_add_compile_definitions(
	${PROJ_NAME}
		-DGLM_FORCE_DEPTH_ZERO_TO_ONE
		-DGLM_ENABLE_EXPERIMENTAL
		-DGLM_FORCE_CTOR_INIT
		PUBLIC
)

if(${MATHUTIL_STATIC})
	pr_add_compile_definitions(${PROJ_NAME} -DMUTIL_STATIC)
endif()

if(${MATHUTIL_ENABLE_MESH_FUNCTIONS})
	pr_add_compile_definitions(${PROJ_NAME} -DENABLE_MESH_FUNCTIONS PUBLIC)
endif()

if(${LINK_COMMON_LIBS_STATIC})
	pr_add_compile_definitions(${PROJ_NAME} -DVFILESYSTEM_STATIC -DSHUTIL_STATIC)
endif()

#if(${MATHUTIL_BUILD_TESTS})
#    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
#	
#    set(TESTS_BINARY_NAME ${PROJ_NAME}_tests)
#	enable_testing()
#	
#	add_subdirectory(${DEPENDENCY_GOOGLE_TESTS_DIR})
#	file(GLOB_RECURSE TESTS_SRC_FILES
#       "${CMAKE_CURRENT_LIST_DIR}/tests/*.cpp"
#    )
#	add_executable(${TESTS_BINARY_NAME} ${TESTS_SRC_FILES})
#	target_link_libraries(${TESTS_BINARY_NAME} PUBLIC ${PROJ_NAME} gtest)
#	target_include_directories(${TESTS_BINARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
#	target_include_directories(${TESTS_BINARY_NAME} PRIVATE ${DEPENDENCY_GLM_INCLUDE})
#	target_include_directories(${TESTS_BINARY_NAME} PRIVATE ${DEPENDENCY_SHAREDUTILS_INCLUDE})
#	target_include_directories(${TESTS_BINARY_NAME} PRIVATE ${DEPENDENCY_GEOMETRIC_TOOLS_INCLUDE})
#	
#	add_test(NAME ${TESTS_BINARY_NAME} COMMAND ${TESTS_BINARY_NAME})
#endif()

pr_finalize(${PROJ_NAME})

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# metadata
set(META_PROJECT_NAME reflective_rapidjson_generator)
set(META_PROJECT_TYPE application)
set(LINK_TESTS_AGAINST_APP_TARGET ON)

# add project files
set(HEADER_FILES
    codegenerator.h
    jsonserializationcodegenerator.h
    codefactory.h
    frontendaction.h
    consumer.h
    visitor.h
    clangversionabstraction.h
)
set(SRC_FILES
    codegenerator.cpp
    jsonserializationcodegenerator.cpp
    codefactory.cpp
    frontendaction.cpp
    consumer.cpp
    clangversionabstraction.cpp
    visitor.cpp
    main.cpp
)
set(TEST_HEADER_FILES
    tests/helper.h
)
set(TEST_SRC_FILES
    tests/cppunit.cpp
)

# add JSON-specific test cases
if(RapidJSON_FOUND)
    list(APPEND TEST_HEADER_FILES
        tests/structs.h
        tests/morestructs.h
    )
    list(APPEND TEST_SRC_FILES
        tests/jsongenerator.cpp
    )
endif()

# link against c++utilities
use_cpp_utilities()

# find Clang for LibTooling; adding clangTooling should be sufficient as it pulls all transitive dependencies
find_package(Clang REQUIRED)
list(APPEND PRIVATE_LIBRARIES clangTooling)

# also add reflective_rapidjson which is header-only but might pull additional include dirs for RapidJSON
list(APPEND PRIVATE_LIBRARIES reflective_rapidjson)

# include modules to apply configuration
include(BasicConfig)
include(WindowsResources)
include(AppTarget)

# trigger code generator for tests because the tests already contain structs to be (de)serialized
include(ReflectionGenerator)
add_reflection_generator_invocation(
    INPUT_FILES
        tests/structs.h                # used by test cases
        tests/morestructs.h            # used by test cases
        tests/cppunit.cpp              # just for testing multiple input files and the "empty file" case
    GENERATORS
        json
    OUTPUT_LISTS
        TEST_HEADER_FILES
    CLANG_OPTIONS_FROM_TARGETS
        reflective_rapidjson_generator_tests
    JSON_CLASSES
        OtherNotJsonSerializable       # test specifying classes for JSON (de)serialization manually
        SomeOtherClassName             # specifying a class that does not exist should not cause any problems
    JSON_VISIBILITY
        LIB_EXPORT                     # not required, just to test setting visibility
)

# include modules to apply configuration
include(TestTarget)
include(ShellCompletion)
include(Doxygen)

# add paths for include dirs of c++utilities and RapidJSON to config header so test cases can use it
set(META_CUSTOM_CONFIG "#define CPP_UTILITIES_INCLUDE_DIRS \"${CPP_UTILITIES_INCLUDE_DIRS}\"\n")
if(RapidJSON_FOUND)
    string(APPEND META_CUSTOM_CONFIG "#define RAPIDJSON_INCLUDE_DIRS \"${RAPIDJSON_INCLUDE_DIRS}\"\n")
endif()

# make config header
include(ConfigHeader)


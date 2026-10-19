cmake_minimum_required(VERSION 3.16)
project(SuperJupiterSensorAgreement VERSION 0.1.1 LANGUAGES CXX)

set(_superjupiter_sensor_agreement_top_level OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(_superjupiter_sensor_agreement_top_level ON)
endif()
option(SUPERJUPITER_SENSOR_AGREEMENT_BUILD_TESTS "Build SensorAgreement tests" ${_superjupiter_sensor_agreement_top_level})
option(SUPERJUPITER_SENSOR_AGREEMENT_BUILD_EXAMPLES "Build SensorAgreement host example" ${_superjupiter_sensor_agreement_top_level})
option(SUPERJUPITER_SENSOR_AGREEMENT_INSTALL "Generate SensorAgreement install rules" ${_superjupiter_sensor_agreement_top_level})

add_library(SuperJupiterSensorAgreement INTERFACE)
add_library(SuperJupiter::SensorAgreement ALIAS SuperJupiterSensorAgreement)
set_target_properties(SuperJupiterSensorAgreement PROPERTIES EXPORT_NAME SensorAgreement)
target_include_directories(SuperJupiterSensorAgreement INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>)
target_compile_features(SuperJupiterSensorAgreement INTERFACE cxx_std_11)

if(SUPERJUPITER_SENSOR_AGREEMENT_BUILD_TESTS)
  enable_testing()
  add_executable(superjupiter-sensor-agreement-review tests/review_regressions.cpp)
  target_link_libraries(superjupiter-sensor-agreement-review PRIVATE SuperJupiter::SensorAgreement)
  add_test(NAME superjupiter-sensor-agreement-review COMMAND superjupiter-sensor-agreement-review)
  add_executable(superjupiter-sensor-agreement-tests tests/test.cpp)
  target_link_libraries(superjupiter-sensor-agreement-tests PRIVATE SuperJupiter::SensorAgreement)
  add_test(NAME superjupiter-sensor-agreement-tests COMMAND superjupiter-sensor-agreement-tests)
  add_executable(superjupiter-sensor-agreement-namespace-test tests/namespace_isolation.cpp)
  target_link_libraries(superjupiter-sensor-agreement-namespace-test PRIVATE SuperJupiter::SensorAgreement)
  add_test(NAME superjupiter-sensor-agreement-namespace-test COMMAND superjupiter-sensor-agreement-namespace-test)
endif()
if(SUPERJUPITER_SENSOR_AGREEMENT_BUILD_EXAMPLES)
  add_executable(superjupiter-sensor-agreement-replay tools/replay.cpp)
  target_link_libraries(superjupiter-sensor-agreement-replay PRIVATE SuperJupiter::SensorAgreement)
endif()
if(SUPERJUPITER_SENSOR_AGREEMENT_INSTALL)
  include(CMakePackageConfigHelpers)
  set(_superjupiter_sensor_agreement_package_dir lib/cmake/SuperJupiterSensorAgreement)
  install(TARGETS SuperJupiterSensorAgreement EXPORT SuperJupiterSensorAgreementTargets)
  install(FILES src/SuperJupiterSensorAgreement.h DESTINATION include)
  install(EXPORT SuperJupiterSensorAgreementTargets NAMESPACE SuperJupiter::
    DESTINATION ${_superjupiter_sensor_agreement_package_dir})
  configure_package_config_file(cmake/SuperJupiterSensorAgreementConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterSensorAgreementConfig.cmake
    INSTALL_DESTINATION ${_superjupiter_sensor_agreement_package_dir})
  write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterSensorAgreementConfigVersion.cmake
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMinorVersion ARCH_INDEPENDENT)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterSensorAgreementConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterSensorAgreementConfigVersion.cmake
    DESTINATION ${_superjupiter_sensor_agreement_package_dir})
  install(FILES LICENSE DESTINATION share/licenses/SuperJupiterSensorAgreement)
endif()

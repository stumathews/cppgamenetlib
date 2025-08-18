cmake_minimum_required(VERSION 4.0)

project(cppgamenetlib VERSION 1.0)

# Restrict to debug and release builds not things like MinSizeRel and RelWithDebInfo
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

include(GenerateExportHeader)

# Include functionality that will help us create the package config for this project
include(CMakePackageConfigHelpers)

enable_testing()

# Use the latest C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create the library using the library source files
add_library(cppgamenetlib STATIC 
EventFactory.cpp
GameClient.cpp
GameClientConnectionFactory.cpp
GameServer.cpp
GameStatePusher.cpp
Message.cpp
Networking.cpp
NetworkingActivityMonitor.cpp
NetworkManager.cpp
NetworkPlayer.cpp
PacketDatum.cpp
PeerInfo.cpp
ReliableUdp.cpp
ReliableUdpGameServerConnection.cpp
ReliableUdpMessageHeader.cpp
ReliableUdpPacketReceivedEvent.cpp
ReliableUdpProtocolManager.cpp
Security.cpp
SerializationManager.cpp
TcpGameServerConnection.cpp
TcpNetworkPlayer.cpp
TcpNetworkSocket.cpp
UdpConnectedNetworkSocket.cpp
UdpGameServerConnection.cpp
UdpNetworkPlayer.cpp
XmlEventSerializationManager.cpp
JsonEventSerializationManager.cpp
BitPackedEventSerializationManager.cpp
NetworkTrafficReceivedEvent.cpp
)


add_library(cppgamenetlib::cppgamenetlib ALIAS cppgamenetlib)

target_sources(cppgamenetlib
 PUBLIC
  FILE_SET api
  TYPE HEADERS
  FILES
BitFiddler.h
BitfieldReader.h
BitPackedEventSerializationManager.h
BitPacker.h
BitPackingTypes.h
crc.h
EventFactory.h
Fish.h
GameClient.h
GameClientConnectionFactory.h
GameServer.h
GameServerConnectionFactory.h
GameStatePusher.h
IConnectedNetworkSocket.h
IElapsedTimeProvider.h
IEventSerializationManager.h
IGameServerConnection.h
IProtocolManager.h
json11.h
JsonEventSerializationManager.h
Message.h
MessageHeader.h
Networking.h
NetworkingActivityMonitor.h
NetworkingStatistics.h
NetworkManager.h
NetworkPlayer.h
NetworkPlayerJoinedEvent.h
NetworkTrafficReceivedEvent.h
PacketDatum.h
PacketDatumUtils.h
PeerInfo.h
PingMessage.h
PongMessage.h
ReliableUdp.h
ReliableUdpAckPacketEvent.h
ReliableUdpCheckSumFailedEvent.h
ReliableUdpGameServerConnection.h
ReliableUdpMessageHeader.h
ReliableUdpPacketLossDetectedEvent.h
ReliableUdpPacketReceivedEvent.h
ReliableUdpPacketRttCalculatedEvent.h
ReliableUdpProtocolManager.h
RingBuffer.h
Rtt.h
Security.h
SerializationManager.h
StartNetworkLevelEvent.h
Statistics.h
TcpGameServerConnection.h
TcpNetworkPlayer.h
TcpNetworkSocket.h
TransportOnlyProtocolManager.h
UdpConnectedNetworkSocket.h
UdpGameServerConnection.h
UdpNetworkPlayer.h
XmlEventSerializationManager.h
)

# Generate a header file containing preprocessor macro definitions to control C/C++ symbol visibility.
generate_export_header(cppgamenetlib)

# Find packages for library dependencies (these will come via vcpkg)
find_package(GTest CONFIG REQUIRED)
find_package(cppgamelib CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)

# Set a few properties for the library  
set_target_properties(cppgamenetlib PROPERTIES LINKER_LANGUAGE CXX)

# Link to external dependencies
target_link_libraries(cppgamenetlib 
PRIVATE
SDL2::SDL2
SDL2_ttf::SDL2_ttf
SDL2_mixer::SDL2_mixer
SDL2_image::SDL2_image
GTest::gmock_main 
winmm
cppgamelib::cppgamelib
) 

# Make an executable that runs all the tests the provided files
enable_testing()

# Add an executable for running all tests. This excludes networking tests
add_executable(AllTests
tests/BitFiddlerTests.cpp 
tests/CrcTests.cpp
tests/RingBufferTests.cpp
tests/ReliableUdpTests.cpp
tests/SecurityTests.cpp
Tests/NetworkingTests.cpp
)

# Set the properties for the test executables
set_target_properties(AllTests PROPERTIES LINKER_LANGUAGE CXX)

# Link libraries that the test AllTests exeutable uses
target_link_libraries(AllTests 
PRIVATE 
cppgamelib::cppgamelib
cppgamenetlib
unofficial-sodium::sodium
PRIVATE
GTest::gmock_main
winmm 

)

# Set include directory for allTests executable
target_include_directories(AllTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Copy test files in the output folder of the AllTests target
add_custom_command(
  TARGET AllTests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_CURRENT_SOURCE_DIR}/testdata/"
          "$<TARGET_FILE_DIR:AllTests>"
)

# Note: We set the working dir of the test to the target output folder otherwise is runs tests from builddir and can't find the configuration files as they are not in the build dir, but the target dir (eg. Release/ etc)
add_test(NAME MainTests COMMAND AllTests WORKING_DIRECTORY $<TARGET_FILE_DIR:AllTests>)


install(TARGETS cppgamenetlib 
EXPORT cppgamenetlib # produce target exports for this target
FILE_SET api 
DESTINATION include/cppgamenetlib
INCLUDES DESTINATION include)

# Create/write the targets exports to the specified file name in the lib directory (as this is where downstream packages look for it via find_package())

install(EXPORT cppgamenetlib
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppgamenetlib
NAMESPACE cppgamenetlib::
)

# Write a basic package config version file for us please
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/cppgamenetlib/cppgamenetlibConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# set a variable for with location of where we are going to install the config package to

set(ConfigPackageLocation lib/cmake/cppgamenetlib)

# install the package config file (this imports the exported target file previously created) and the version file
install(
  FILES
    cmake/cppgamenetlibConfig.cmake # This file must be created by us
    "${CMAKE_CURRENT_BINARY_DIR}/cppgamenetlib/cppgamenetlibConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)

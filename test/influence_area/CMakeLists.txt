cmake_minimum_required(VERSION 3.5.1)

# プロジェクト名
set(project_name influence_area_check)
project(${project_name} CXX)

# Build Type(Release or Debug)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

# ccache
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

# コンパイルオプション
add_definitions("-Wall -std=c++14")

# インクルードパス
include_directories($ENV{REALCORE_DIR}/src/include)

# ライブラリパス
link_directories($ENV{BOOST_DIR}/lib)

# ソースファイル
add_executable(
    ${project_name}
    $ENV{REALCORE_DIR}/src/CSVReader.cc
    $ENV{REALCORE_DIR}/src/Move.cc
    $ENV{REALCORE_DIR}/src/MoveList.cc
    $ENV{REALCORE_DIR}/src/LineNeighborhood.cc
    $ENV{REALCORE_DIR}/src/OpenState.cc
    $ENV{REALCORE_DIR}/src/BoardOpenState.cc
    $ENV{REALCORE_DIR}/src/BitBoard.cc
    $ENV{REALCORE_DIR}/src/Board.cc
    ../InfluenceAreaCheck.cc
)

# ライブラリ
target_link_libraries(${project_name} boost_program_options)

if(APPLE OR WIN32)
  target_link_libraries(${project_name} boost_system-mt)
  target_link_libraries(${project_name} boost_thread-mt)
else()
  target_link_libraries(${project_name} boost_system)
  target_link_libraries(${project_name} boost_thread)
endif()


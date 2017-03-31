cmake_minimum_required(VERSION 3.5.1)

# プロジェクト名
set(project_name board_get_set)
project(${project_name} CXX)

# Build Type(Release or Debug)
set(CMAKE_BUILD_TYPE Release)
# message(STATUS "${CMAKE_CXX_FLAGS_RELEASE}")

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
include_directories($ENV{GTEST_DIR}/googletest/include)

# ライブラリパス
link_directories($ENV{BOOST_DIR}/lib)

# ソースファイル
add_executable(
    ${project_name}
    $ENV{REALCORE_DIR}/src/RealCore.cc
    $ENV{REALCORE_DIR}/src/Move.cc
    $ENV{REALCORE_DIR}/src/BitBoard.cc
    ../BoardGetSet.cc
)

# ライブラリ
target_link_libraries(${project_name} boost_program_options)


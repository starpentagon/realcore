cmake_minimum_required(VERSION 3.5.1)

# プロジェクト名
set(project_name bit_board_test)
project(${project_name} CXX)

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
link_directories($ENV{GTEST_DIR}/googlemock/gtest)

# ソースファイル
add_executable(
    ${project_name}
    $ENV{REALCORE_DIR}/src/RealCore.cc
    $ENV{REALCORE_DIR}/src/BitSearch.cc
    $ENV{REALCORE_DIR}/src/Move.cc
    $ENV{REALCORE_DIR}/src/MoveList.cc
    $ENV{REALCORE_DIR}/src/BitBoard.cc
    ../BitBoardTest.cc
    ../BitBoardMemberTest.cc
    ../BitBoardForbiddenTest.cc
)

# ライブラリ
target_link_libraries(${project_name} gtest)
target_link_libraries(${project_name} gtest_main)

# Ubuntuでgoogle testをビルドするためにはpthreadが必要
target_link_libraries(${project_name} pthread)


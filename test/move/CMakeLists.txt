cmake_minimum_required(VERSION 3.7.2)

# プロジェクト名
set(project_name move_test)

project(${project_name} CXX)

add_definitions("-Wall -std=c++14")

# インクルードパス
include_directories($ENV{REALCORE_DIR}/src/include)
include_directories($ENV{GTEST_DIR}/googletest/include)

# ライブラリパス
link_directories($ENV{GTEST_DIR}/googlemock/gtest)

# ソースファイル
add_executable(
    ${project_name}
    $ENV{REALCORE_DIR}/src/Move.cc
    ../MoveTest.cc
)

# ライブラリ
target_link_libraries(${project_name} gtest)
target_link_libraries(${project_name} gtest_main)


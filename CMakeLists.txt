cmake_minimum_required(VERSION 2.8.3)
project(step1)
set(SOURCE_FILE src/main.cpp)

# OpenCVのビルド環境の設定ファイルを読み込む
find_package(OpenCV REQUIRED)

# OpenCV関係のインクルードディレクトリのパスを設定
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)
set(
    SOURCES
    src/main.cpp
    src/ImageProcess.cpp
    src/ParseJSON.cpp
    src/CalculateHRR.cpp
)

# 実行ファイル名を設定
add_executable(main
    ${SOURCE_FILE}
    ${SOURCES}
)

# リンクするライブラリを読み込む
target_link_libraries(main
    ${OpenCV_LIBRARIES}
)
# デバッグ用のフラグ（コンパイル時に -g を確実に追加）
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0") 

# src/ ディレクトリにCMakeLists.txtがあれば追加
add_subdirectory(src)
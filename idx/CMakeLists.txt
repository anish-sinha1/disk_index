add_library(idx src/song/song.c include/song/song.h)
target_include_directories(idx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include /usr/local/include)
link_directories(/usr/local/lib)
target_link_libraries(idx PUBLIC cslice)
# 这是一个项目空间的CMakeList
# 空间内可能包含多个工程
# 每个工程被定义为一子文件夹
# 工程公共部分被定义在common工程下
include_directories(common)
add_subdirectory(common)

set(LIBS ${LIBS} common_static_lib)

# 如果编译外部链接库，请从下层编译，注意引用顺序
add_subdirectory(test)


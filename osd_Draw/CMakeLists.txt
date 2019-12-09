#======================================== 库的生成 =================================================================
# 头文件/源文件包含路径
include_directories(./)
aux_source_directory(./ src_base_List)

set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -O2 ")

#  libneonOsd.so 和 libneonOsd.a
add_library(${Library_OutPutName}_shared SHARED ${src_base_List}) # add_library 模块名字不能一样;
add_library(${Library_OutPutName}_static STATIC ${src_base_List})
set_target_properties(${Library_OutPutName}_shared PROPERTIES OUTPUT_NAME ${Library_OutPutName})  # 重命名 add_library 中的模块名字
set_target_properties(${Library_OutPutName}_static PROPERTIES OUTPUT_NAME ${Library_OutPutName})

# 设置动态库版本号, 主要是为了解决 linux 上的 dll-hell 问题;
#  [LINUX下动态库及版本号控制](https://blog.csdn.net/David_xtd/article/details/7045792)
# {libname.so.x.y.z (real name) ->  x是主版本号(Major Version Number)，y是次版本号(Minor Version Number)，z是发布版本号(Release Version Number) }
set_target_properties(${Library_OutPutName}_shared PROPERTIES VERSION 1.2.3 SOVERSION 1) 

# 安装共享库和头文件:
# 设置库文件文件输出目录
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/install/lib) 
install(TARGETS ${Library_OutPutName}_shared ${Library_OutPutName}_static
		LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/install/lib
		ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/install/lib)
file(GLOB lib_include_files ./*.h)
install(FILES ${lib_include_files} 
		DESTINATION ${PROJECT_SOURCE_DIR}/install/include)

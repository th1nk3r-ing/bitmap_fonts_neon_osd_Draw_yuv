#====================================================================================================================
# 头文件/源文件包含路径
include_directories(./inc/)
include_directories(${PROJECT_SOURCE_DIR}/osd_Draw/)

# 把当前 src下所有源代码文件和头文件加入变量中
aux_source_directory(./src/ src_demo_List)

#========================================== 测试 demo 的生成 =======================================================
# 链接文件夹路径 (使用绝对路径可避免报错)
link_directories(${PROJECT_BINARY_DIR}/lib)
# link_libraries -> Link libraries to all targets added later. 
# 静态库 (add_executable 之前指定) 
link_libraries(lib${Library_OutPutName}.a)
add_executable(${My_Target} ${src_demo_List})

# 配置依赖关系(需在对应 target 添加之后进行配置);
add_dependencies( ${My_Target} ${Library_OutPutName}_static ${Library_OutPutName}_shared)

target_link_libraries(${My_Target} -lstdc++ -lm -ldl )
#target_link_libraries(${My_Target} -l${Library_OutPutName}) # 使用动态库

# make install
install(TARGETS ${My_Target} DESTINATION ${PROJECT_SOURCE_DIR}/install
		PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE)
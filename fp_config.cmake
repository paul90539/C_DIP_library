
#set dir_core
set( dir_core              ${CMAKE_CURRENT_SOURCE_DIR}/../module/core)

#set dir_algorithm
set( dir_algorithm         ${CMAKE_CURRENT_SOURCE_DIR}/../module/algorithm)

#set dir_dip_lib_test
set( dir_dip_lib_test      ${CMAKE_CURRENT_SOURCE_DIR}/../dip_lib_test)    

#set src_core
foreach( dir ${dir_core} )
    aux_source_directory( ${dir}/src src_core )
    file(GLOB_RECURSE CURRENT_HEADERS  ${dir}/inc/*.h  ${dir}/inc/*.hpp)
    set( inc_core ${inc_core} ${CURRENT_HEADERS} )
endforeach()

#set dir_algorithm
foreach( dir ${dir_algorithm} )
    aux_source_directory( ${dir}/src src_algorithm )
    file(GLOB_RECURSE CURRENT_HEADERS  ${dir}/inc/*.h  ${dir}/inc/*.hpp)
    set( inc_algorithm ${inc_algorithm} ${CURRENT_HEADERS} )
endforeach()


#set src_dip_lib_test
foreach( dir ${dir_dip_lib_test} )
    aux_source_directory( ${dir}/src src_dip_lib_test )
    file(GLOB_RECURSE CURRENT_HEADERS  ${dir}/inc/*.h  ${dir}/inc/*.hpp)
    set( inc_dip_lib_test ${inc_dip_lib_test} ${CURRENT_HEADERS} )
endforeach()


MACRO(DEFINE_DIP_LIB_EXE name)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

source_group("Include Files\\core" FILES ${inc_core})
source_group( "Source Files\\core" FILES ${src_core})

source_group("Include Files\\alg" FILES ${inc_algorithm})
source_group( "Source Files\\alg" FILES ${src_algorithm})

source_group("Include Files" FILES ${inc_dip_lib_test})
source_group( "Source Files" FILES ${src_dip_lib_test})

add_executable(${name}  ${inc_core} ${src_core} 
                        ${inc_algorithm}  ${src_algorithm}
                        ${inc_dip_lib_test} ${src_dip_lib_test}  )

target_compile_definitions(${name} PRIVATE _FTS_ALGO)
target_include_directories(${name} PRIVATE 
         ${dir_core}/inc
         ${dir_algorithm}/inc
         ${dir_dip_lib_test}/inc
    )

ENDMACRO()


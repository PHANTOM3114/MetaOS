find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)

set(PROTO_FILE "agent.proto")
set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated_proto_files)

file(MAKE_DIRECTORY ${GENERATED_DIR})

get_filename_component(PROTO_FILENAME ${PROTO_FILE} NAME_WE)
set(GENERATED_PB_H ${GENERATED_DIR}/${PROTO_FILENAME}.pb.h)
set(GENERATED_PB_CC ${GENERATED_DIR}/${PROTO_FILENAME}.pb.cc)
set(GENERATED_GRPC_H ${GENERATED_DIR}/${PROTO_FILENAME}.grpc.pb.h)
set(GENERATED_GRPC_CC ${GENERATED_DIR}/${PROTO_FILENAME}.grpc.pb.cc)

add_custom_command(
        OUTPUT
        ${GENERATED_PB_H} ${GENERATED_PB_CC}
        ${GENERATED_GRPC_H} ${GENERATED_GRPC_CC}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR}
        COMMAND protobuf::protoc
        --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
        --cpp_out=${GENERATED_DIR}
        --grpc_out=${GENERATED_DIR}
        --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
        ${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
)

add_library(agent_proto_lib
        ${GENERATED_PB_CC}
        ${GENERATED_GRPC_CC}
)

target_include_directories(agent_proto_lib PUBLIC
        ${GENERATED_DIR}
)

target_link_libraries(agent_proto_lib PUBLIC
        protobuf::libprotobuf
        gRPC::grpc++
)
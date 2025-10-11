add_library(server_logic
        shell_controller/src/shell_controller.cc
)

target_include_directories(server_logic PUBLIC
        shell_controller/include
)

target_link_libraries(server_logic PUBLIC
        generated_protos
        gRPC::grpc++
        protobuf::libprotobuf
)

add_executable(server
        main.cc
)

target_link_libraries(server PRIVATE
        server_logic
        gRPC::grpc++_reflection
)
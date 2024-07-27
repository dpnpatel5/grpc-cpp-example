# Compiler and flags
CFLAGS += -Wall -Werror -Wno-error=date-time
CXXFLAGS += -Werror=return-type -g -fPIC -Wall -Werror -Wno-error=date-time
LDFLAGS_NEW = $(LDFLAGS)
LDFLAGS_NEW +=  `pkg-config --libs protobuf grpc++` \
               -pthread \
               -L/home/dipatel/practise/gRPCApplication/pkgconfig \
		-L/usr/local/lib \
               -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed \
               -ldl -no-pie -rdynamic -lupb_mem_lib -lupb_message_lib -lupb_base_lib -lupb_textformat_lib -lupb_json_lib

# Paths
PROTOC = /home/dipatel/practise/gRPCApplication/grpc/cmake/build/third_party/protobuf/protoc
GRPC_CPP_PLUGIN = /home/dipatel/practise/gRPCApplication/grpc/cmake/build/grpc_cpp_plugin
PROTOS_PATH = -I./

# Include paths
GRPC_INCLUDE_PATH = /home/dipatel/practise/gRPCApplication/grpc/include
PROTOBUF_INCLUDE_PATH = /home/dipatel/practise/gRPCApplication/grpc/third_party/protobuf/src
ABSEIL_INCLUDE_PATH = /home/dipatel/practise/gRPCApplication/grpc/third_party/abseil-cpp

# Include paths for compilation
CXXFLAGS += -I$(GRPC_INCLUDE_PATH) -I$(PROTOBUF_INCLUDE_PATH) -I$(ABSEIL_INCLUDE_PATH)

# Targets
all: calculator_server calculator_client

# Server target
calculator_server: calculator.pb.o calculator.grpc.pb.o calculator_server.o
	$(CXX) $^ $(LDFLAGS_NEW) -o $@

# Client target
calculator_client: calculator.pb.o calculator.grpc.pb.o calculator_client.o 
	$(CXX) $^ $(LDFLAGS_NEW) -o $@

# Object file rules
%.o: %.cpp
	$(CXX) -o $@ -c $(CPPFLAGS) $(CXXFLAGS) $<

# Generate Protobuf and gRPC source files
#$(PROTO_SRC) $(GRPC_PROTO_SRC): $(PROTO_FILE)
%.pb.cc %.pb.h %.grpc.pb.cc %.grpc.pb.h: %.proto
	$(PROTOC) $(PROTOS_PATH) --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN) $<

# Clean up
clean:
	rm -f *.o *.pb.cc *.pb.h *.grpc.pb.cc *.grpc.pb.h calculator_server calculator_client

# Git version header
git_ver.h:
	echo "#define GIT_VER \\" > git_ver.h
	echo "\"`git rev-parse --short HEAD`\"" >> git_ver.h


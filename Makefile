PROTOBUF_ABSL_DEPS = absl_absl_check absl_absl_log absl_algorithm absl_base absl_bind_front absl_bits absl_btree absl_cleanup absl_cord absl_core_headers absl_debugging absl_die_if_null absl_dynamic_annotations absl_flags absl_flat_hash_map absl_flat_hash_set absl_function_ref absl_hash absl_layout absl_log_initialize absl_log_severity absl_memory absl_node_hash_map absl_node_hash_set absl_optional absl_span absl_status absl_statusor absl_strings absl_synchronization absl_time absl_type_traits absl_utility absl_variant

PROTOBUF_UTF8_RANGE_LINK_LIBS = -lutf8_validity

CPPFLAGS += `pkg-config --cflags protobuf absl_flags absl_flags_parse`
LDFLAGS += `pkg-config --libs --static protobuf grpc++ absl_flags absl_flags_parse $(PROTOBUF_ABSL_DEPS)`\
           $(PROTOBUF_UTF8_RANGE_LINK_LIBS) \
           -pthread\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
	   -ldl

# Paths
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
PROTOS_PATH = -I./
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`


# Targets
all: calculator_server calculator_client

# Server target
calculator_server: calculator.pb.o calculator.grpc.pb.o calculator_server.o
	$(CXX) $^ $(LDFLAGS) -o $@

# Client target
calculator_client: calculator.pb.o calculator.grpc.pb.o calculator_client.o calculator_api.o 
	$(CXX) $^ $(LDFLAGS) -o $@

# Object file rules
%.o: %.cpp
	$(CXX) -o $@ -c $(CPPFLAGS) $(CXXFLAGS) $<

%.o: %.c
	$(CXX) -o $@ -c $(CFLAGS) $^

# Generate Protobuf and gRPC source files
%.pb.cc %.pb.h %.grpc.pb.cc %.grpc.pb.h: %.proto
	$(PROTOC) $(PROTOS_PATH) --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

# Clean up
clean:
	rm -f *.o *.pb.cc *.pb.h *.grpc.pb.cc *.grpc.pb.h calculator_server calculator_client


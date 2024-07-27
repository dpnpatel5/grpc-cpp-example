// calculator_client.cpp
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "calculator_rpc.grpc.pb.h"

int main(int argc, char** argv) {
  std::string target_str = "localhost:50051";
  Calculator::Stub stub = Calculator::NewStub(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  AddRequest request;
  request.set_a(3);
  request.set_b(4);

  AddReply reply;
  grpc::ClientContext context;

  grpc::Status status = stub.Add(&context, request, &reply);

  if (status.ok()) {
    std::cout << "Sum: " << reply.result() << std::endl;
  } else {
    std::cout << "gRPC call failed" << std::endl;
  }

  return 0;
}


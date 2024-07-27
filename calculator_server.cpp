// calculator_client.cpp
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "calculator.grpc.pb.h"

using namespace calculator_rpc;

class CalculatorServiceImpl final : public Calculator::Service {
	grpc::Status Add(grpc::ServerContext* context, const AddRequest* request, AddReply* reply) override {
		int result = request->a() + request->b();
		reply->set_result(result);
		return grpc::Status::OK;
	}
	grpc::Status Sub(grpc::ServerContext* context, const SubRequest* request, SubReply* reply) override {
		int result = request->a() - request->b();
		reply->set_result(result);
		return grpc::Status::OK;
	}
	grpc::Status Mul(grpc::ServerContext* context, const MulRequest* request, MulReply* reply) override {
		int result = request->a() * request->b();
		reply->set_result(result);
		return grpc::Status::OK;
	}
	grpc::Status Div(grpc::ServerContext* context, const DivRequest* request, DivReply* reply) override {
		int result = request->a() / request->b();
		reply->set_result(result);
		return grpc::Status::OK;
	}
};

void RunServer() {
	std::string server_address("0.0.0.0:50051");
	CalculatorServiceImpl service;

	grpc::ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;
	server->Wait();
}

int main(int argc, char** argv) {
	RunServer();
	return 0;
}


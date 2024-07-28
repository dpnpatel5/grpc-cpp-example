
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>
#include <cstring>
#include <mutex>
#include <errno.h>

#include <grpcpp/grpcpp.h>

#include "calculator.grpc.pb.h"
#include "calculator_api.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using namespace google::protobuf;
using namespace calculator_rpc;
using namespace std;


class CalculatorClient {
	private:
		std::unique_ptr<Calculator::Stub> stub_;

	public:
		static const int ERR_CODE_BASE = 100;

		CalculatorClient(std::shared_ptr<Channel> channel)
			: stub_(Calculator::NewStub(channel)) {}

		int err_code(Status& status) {
			return -(ERR_CODE_BASE + status.error_code());
		}

		int client_add(int a ,int b) {
			AddRequest request;
			// google::protobuf::Empty req;
			request.set_a(a);
			request.set_b(b);

			AddReply reply;
			grpc::ClientContext context;

			grpc::Status status = stub_->Add(&context, request, &reply);

			if (status.ok()) {
				return reply.result();
			} else {
				return err_code(status);
			}
		}
		int client_sub(int a ,int b) {
			SubRequest request;
			// google::protobuf::Empty req;
			request.set_a(a);
			request.set_b(b);

			SubReply reply;
			grpc::ClientContext context;

			grpc::Status status = stub_->Sub(&context, request, &reply);

			if (status.ok()) {
				return reply.result();
			} else {
				return err_code(status);
			}
		}
		int client_mul(int a ,int b) {
			MulRequest request;
			// google::protobuf::Empty req;
			request.set_a(a);
			request.set_b(b);

			MulReply reply;
			grpc::ClientContext context;

			grpc::Status status = stub_->Mul(&context, request, &reply);

			if (status.ok()) {
				return reply.result();
			} else {
				return err_code(status);
			}
		}
		int client_div(int a ,int b) {
			DivRequest request;
			// google::protobuf::Empty req;
			request.set_a(a);
			request.set_b(b);

			DivReply reply;
			grpc::ClientContext context;

			grpc::Status status = stub_->Div(&context, request, &reply);

			if (status.ok()) {
				return reply.result();
			} else {
				return err_code(status);
			}
		}

};

static CalculatorClient * calculator = NULL;

int connect_server(const char * target_str)
{
	if (NULL == target_str) {
		std::cerr << "Error: target_str is NULL" << std::endl;
		return -1;
	}

	if (NULL != calculator) {
		std::cerr << "Client is already initialized" << std::endl;
		return 0;
	}

	try {
		/* Below Line commented because stub as been include as part of CalculatorClient Class */
		// std::unique_ptr<Calculator::Stub> stub = Calculator::NewStub(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

		// Create the channel and initialize the client
		auto channel = grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials());
		if (!channel) {
			std::cerr << "Error: Failed to create gRPC channel" << std::endl;
			return -1;
		}

		// Check channel state
		grpc_connectivity_state state;
		for (int i = 0; i < 10; ++i) { // Check up to 10 times
			state = channel->GetState(true);
			if (state == GRPC_CHANNEL_READY) {
				break;
			}
			std::cerr << "Channel state: " << state << ". Waiting for READY state..." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}


		// Create the client
		calculator = new CalculatorClient(channel);
		if (NULL == calculator) {
			std::cerr << "Error: Failed to create CalculatorClient" << std::endl;
			return -1;
		}

		std::cout << "Successfully connected to server at " << target_str << std::endl;
		return 0;
	} catch (const std::exception &e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		return -1;
	}


}
int add_api(int a , int b)
{
	return calculator->client_add(a,b);
}
int sub_api(int a , int b)
{
	return calculator->client_sub(a,b);
}
int mul_api(int a , int b)
{
	return calculator->client_mul(a,b);
}
int div_api(int a , int b)
{
	return calculator->client_div(a,b);
}

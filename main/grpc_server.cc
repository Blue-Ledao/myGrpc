#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "main/demo.grpc.pb.h"
#else
#include "main/demo.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using mygrpc::UserServer;
using mygrpc::User;
using mygrpc::Reply;
using namespace std;

class LoginServiceImpl final : public UserServer::Service {

    Status Login(ServerContext* context, const User* user,
                    Reply* reply) override {
        
        return Status::OK;
    }
    
    Status Add(ServerContext* context, const User* user,
                 Reply* reply) override {
        
        return Status::OK;
    }
    Status UserStatusListen (ServerContext* context,
                     ServerReaderWriter<User, User>* stream) override {
        
      return Status::OK;
    }
    
private:
    std::mutex mu_;
    std::vector<User> login_users;
    std::vector<ServerReaderWriter<User, User>*> client_streams;
    std::map<string, string> user_map;
};

void RunServer() {
  std::string address = "0.0.0.0";
  std::string port = "50051";
  std::string server_address = address + ":" + port;
  LoginServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;


  server->Wait();
}

int main(int argc, char** argv) {
    RunServer();

    return 0;
}

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "main/demo.grpc.pb.h"
#else
#include "main/demo.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::ClientReaderWriter;
using grpc::Status;
using mygrpc::UserServer;
using mygrpc::User;
using mygrpc::Reply;
using namespace std;


class LoginClient {
 public:
    LoginClient(std::shared_ptr<Channel> channel)
        : stub_(UserServer::NewStub(channel)) {}

    std::string login(const std::string& username, const std::string& pwd) {
      
    }
    
    std::string add(const std::string& username, const std::string& pwd) {
      
    }
    
    void userStatusListen() {
        
    }
    
private:
    std::unique_ptr<UserServer::Stub> stub_;
};

int main(int argc, char** argv) {
    
  return 0;
}





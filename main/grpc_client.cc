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

const string USERNAME_OR_PASSWORD_ERROR = "用户名或者密码错误";
const string ADD_ERROR = "ADD_ERROR";
User currentUser;

class LoginClient {
 public:
    LoginClient(std::shared_ptr<Channel> channel)
        : stub_(UserServer::NewStub(channel)) {}

    std::string login(const std::string& username, const std::string& pwd) {
        User user;
        user.set_name(username);
        user.set_password(pwd);

        
        Reply reply;
        ClientContext context;

        Status status = stub_->Login(&context, user, &reply);

        if (status.ok()) {
            currentUser.set_token(reply.token());
          return reply.message();
        } else {
          std::cout << status.error_code() << ": " << status.error_message()
                    << std::endl;
          return "RPC failed";
        }
    }
    
    std::string add(const std::string& username, const std::string& pwd) {
      
    }
    
    void userStatusListen() {
        
    }
    
private:
    std::unique_ptr<UserServer::Stub> stub_;
};

int main(int argc, char** argv) {
    std::string address = "localhost";
    std::string port = "50051";
    std::string server_address = address + ":" + port;
    std::cout << "Client querying server address: " << server_address << std::endl;
      
    LoginClient loginClient(grpc::CreateChannel(
        server_address, grpc::InsecureChannelCredentials()));
    
    while (true){
          int opt = 0;
          std::cout << "主菜单" << std::endl;
          std::cout << "1 : " << "注册" << std::endl;
          std::cout << "2 : " << "登录系统" << std::endl;
          std::cout << "3 : " << "离开程序" << std::endl;
          std::cin >> opt;
          std::string username;
          std::string pwd;
          switch (opt){
                  case 1:{
                      std::cout << "请输入用户名 : "; std::cin >> username;
                      std::cout << "请输入用密码 : "; std::cin >> pwd;
                                          
                      std::string reply = "";
                      if(reply != ADD_ERROR){
                          std::cout << "注册用户成功！\t"  <<  std::endl;
                      }else{
                          std::cout << "注册用户失败！\t"  <<  std::endl;
                      }
                      
                      break;
                  }
                  case 2:{
                      std::cout << "请输入用户名 : "; std::cin >> username;
                      std::cout << "请输入用密码 : "; std::cin >> pwd;
                      currentUser.set_name(username);
                      
                      std::string reply = loginClient.login(username, pwd);
                      std::cout << reply << std::endl;
                      if(reply != USERNAME_OR_PASSWORD_ERROR){
                          std::cout << "登录成功，token是：" << currentUser.token() << "，输入'logout'退出当前登录，并返回主菜单...." << std::endl;
                          loginClient.userStatusListen();
                          while(true){
                              std::string command;
                              std::cin >> command;
                              if(command == "logout"){
                                  break;
                              }else{
                                  std::cout << "指令非法...." << std::endl;
                              }
                          }
                      }
                      break;
                  }
                  case 3: {
                      return 0;
                  }
          }
      }
    return 0;
}





#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "main/demo.grpc.pb.h"
#else
#include "main/demo.grpc.pb.h"
#endif

#include <ctime>
#include <list>
#include<fstream>
#include<sstream>


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using mygrpc::UserServer;
using mygrpc::User;
using mygrpc::Reply;
using namespace std;

const string fileName = "users.txt";
const string USERNAME_OR_PASSWORD_ERROR = "用户名或者密码错误";
const string NAME_EXIST_ERROR = "用户已存在";

int getMaxId();
bool isDuplicate(const std::string& name, list<User> userList);
string buildToken();
bool queryUser(const std::string& name, const std::string& pwd, list<User> userList);
list<User> loadUserInfo();

class LoginServiceImpl final : public UserServer::Service {

    Status Login(ServerContext* context, const User* user,
                    Reply* reply) override {
        list<User> userList;
        userList = loadUserInfo();

        bool isExist;
        isExist = queryUser(user->name(), user->password(), userList);
        if(isExist){
            const string token = buildToken();
            std::string prefix("Hello ");
            reply->set_message(prefix + user->name());
            reply->set_token(token);
            user_map.insert(pair<string,string>(token, user->name()));
        }else{
            reply->set_message(USERNAME_OR_PASSWORD_ERROR);
        }

        return Status::OK;
    }

    Status Add(ServerContext* context, const User* user,
                 Reply* reply) override {
        list<User> userList;
        userList = loadUserInfo();
        if(isDuplicate(user->name(), userList)){
            reply->set_message(NAME_EXIST_ERROR);
            return Status::OK;
        }
        string id = to_string(getMaxId());
        ofstream file_writer("./" + fileName, ios_base::app);
        file_writer <<  id << setw(10) << user->name() << setw(10) << user->password() << "\t" << endl;
        file_writer.close();
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



/**
 模拟从数据库中加载对象的逻辑，实际上是从文本文件读取
 */
list<User> loadUserInfo(){
    list<User> userList;
    ifstream in;
    in.open(fileName);
    string sline;
    User user;
    while (getline(in, sline))
    {
        string id;
        string name;
        string password;
        istringstream sin(sline);
        sin >> id >> name >> password;

        user.set_id(id);
        user.set_name(name);
        user.set_password(password);
        userList.push_back(user);
    }
    return userList;
}

/**
 查询用户是否存在
 */
bool queryUser(const std::string& name, const std::string& pwd, const list<User> userList){
    list<User>::const_iterator it;
    for(it = userList.begin(); it != userList.end(); it++){
        if(it->name() == name && it->password() == pwd){
            return true;
        }
    }
    return false;
}

/**
 若登录成功，返回给客户端一个token
 */
string buildToken(){
    const int LEN = 62;
    int iLen = 10;
    char g_arrCharElem[LEN] = {
        '0', '1', '2','3', '4', '5','6', '7', '8','9',
        'A', 'B', 'C','D', 'E', 'F','G', 'H', 'I','J',
        'K', 'L', 'M','N', 'O', 'P','Q', 'R', 'S','T',
        'U', 'V', 'W','X', 'Y', 'Z','a', 'b', 'c','d',
        'e', 'f', 'g','h', 'i', 'j','k', 'l', 'm','n',
        'o', 'p', 'q','r', 's', 't','u', 'v', 'w','x',
        'y', 'z'
    };
    char* token = new char[iLen + 1];
    token[iLen] = '\0';
    srand((unsigned)time(0));
    int iRand = 0;
    for (int i = 0; i < iLen; ++i) {
     iRand = rand() % LEN;   // iRand = 0 - 61
     token[i] = g_arrCharElem[iRand];
    }
    delete[] token;
    return token;
}

/**
 获取当前用户中最大的id，添加user时，模拟自增主键
 */
int getMaxId(){
    int maxId = 0;
    list<User> userList;
    userList = loadUserInfo();
    list<User>::const_iterator it;
    int id;
    for(it = userList.begin(); it != userList.end(); it++){
        istringstream ss;
        ss.str(it->id());
        ss >> id;
        
        ofstream file_writer("./id.txt", ios_base::app);
        file_writer <<  it->id() << setw(10)<<  it->name() << setw(10) <<  it->password() << setw(10) <<  id << setw(10) <<  maxId << setw(10) << "\t" << endl;
        file_writer.close();
        
        
        if(maxId < id){
            maxId = id;
        }
    }
    
    return maxId + 1;
}
/**
 注册用户时，判断用户名是否重复
 */
bool isDuplicate(const std::string& name, list<User> userList){
    list<User>::const_iterator it;
    for(it = userList.begin(); it != userList.end(); it++){
        if(it->name() == name){
            return true;
        }
    }
    return false;
}



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

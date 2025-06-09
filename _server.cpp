#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring> 
#include <algorithm>
#include <cctype>
#include <ctime>
#include <csignal>
#include <cstdlib>
#include <map>

#include "lib/tcc.hpp"
#include "lib/nwf.hpp" // MEW WRITE FILE
#include "lib/rwc.hpp" // READ WRITE CONFIG
#include "lib/fnp.hpp" // FUNCTIONS PATHS

#include "lib/kapi.hpp"


void client_session(int client_socket) {

    char buffer[1024];
    int bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
    std::time_t ke_time;
    std::string ip_requests;
    std::string path_requrests;
    std::string user_agent_reauests;
  
    if (bytes_read > 0) {
       
        get_form userData = verbos_lib(client_socket,buffer);
        
        ke_time=userData.user_data.time;
        path_requrests=userData.path;
        ip_requests=userData.user_data.user_ip;
        user_agent_reauests=userData.user_data.user_agent;

        path_requrests = ekran_hash_sims(path_requrests);
        std::string outServerName=(path.server!=""?path.server:path.standard_name);

        get_data_form return_client_data  = get_data_in_url(path_requrests);
        
        if(path.kairrer){
            if(file_type(path_requrests,"kr")){
                krr_respons prcode=kawer(return_client_data.data,userData);
                return_client_data.data=prcode.body;
            }
        }
        
        std::string outContentType = build_contentType(return_client_data.full_url,path.content_type);
        
        int status_code;
        if (return_client_data.code == GET_DATA_OK ) {
            std::string content = return_client_data.data;

            std::string response = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type:"+outContentType+" \r\n" 
                "Server: "+path.server+"\r\n"
                "Content-Length: " + std::to_string(content.length()) + "\r\n\r\n" + content;


            send(client_socket, response.c_str(), response.length(), 0);
            status_code=200;
        
        }
        else if (return_client_data.code==GET_DATA_FORBIDDEN){
            // 403 (Forbidden)
            std::string response = 
            "HTTP/1.1 403 Forbidden\r\n"
            "Content-Type: text/html\r\n" 
            "Server: "+path.server+"\r\n"
            "Content-Length: 0\r\n\r\n";


        send(client_socket, response.c_str(), response.length(), 0);
        status_code=403;
        }
        else if (return_client_data.code == GET_DATA_NOTHING ) {
            // 404 (Not Found)
            std::string response = 
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html\r\n" 
                "Server: "+path.server+"\r\n"
                "Content-Length: 0\r\n\r\n";


            send(client_socket, response.c_str(), response.length(), 0);
            status_code=404;
        }
        else if (return_client_data.code == GET_DATA_EL_ERROR ) {
            // 500 (Internal Server Error)
            std::string response =
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/html\r\n"
            "Server: "+path.server+"\r\n"
            "Content-Length: 0\r\n\r\n";
            send(client_socket, response.c_str(), response.length(), 0);
            status_code=500;
        }
        std::string color_path=(path.super_buff ? cyan : reset);

        std::cout<<"|["<<ke_time<<"]|[" << getColorStatus_code(status_code)
              << status_code << reset
              << "]|[" << ip_requests << "]|"<<magenta<<"["<<reset <<color_path<< path_requrests <<color_path<<magenta<< "]"<<reset<<"|["
              <<blue <<user_agent_reauests <<reset<< " ]" << std::endl;
    
    
    }

    if (client_socket != STATUS_SYSTEM_OPERATION_ERROR) close(client_socket);

}




int main(int argc, char* argv[]) {
     // =============================================================== WELCOME ===============================================================
    std::signal(SIGINT, zexcWer); 
    std::signal(SIGTERM, zexcWer); 
    ConfigData config;
    if (argc < 2) {
        std::string standard_name=path.standard_name;
        std::string standard_file_name_conf = standard_name+".conf";
        config = read_config(standard_file_name_conf);
        if (config.port.length() <1 && config.index.length() <1){
            std::string ok;
            std::cout<<"|[ANS][ERR]|[BAD CONFIG][ "<<standard_file_name_conf<<" ]"<<std::endl<<"|[ANS][MSS]|[ "<<cyan<<argv[0]<<" --help"<<reset<<" ]"<<std::endl<<"|[ANS][MSS]|[BAD CONFIG]| want to create a default host? [y/n]: ";
            std::cin  >>ok;
        
            if (ok == "y"){
                new_dir(standard_name);
                new_file(standard_file_name_conf,path.default_conf);
                new_file(standard_name+"/index.html",path.default_page);
                config = read_config(standard_file_name_conf);
            }
            else{
                std::cerr<<"abort"<<std::endl;
                return STATUS_OPERATION_ERROR;
            }
            
        }
    }
    else if (argc == 2) {
        auto marg=std::string(argv[1]) ;
        if (marg == "--help" || marg == "-help"  || marg == "--h"  || marg== "-h" || marg == "help") {
            std::cout << "|[ANS][HELP]|[ "<<std::endl<<cyan<<"\t" << argv[0] << " <filename>.conf"<<reset<<std::endl<<" ]"<<std::endl
            <<std::endl<<"\t"<<cyan<<argv[0] <<" --v "<<reset<<" all intormation"<<std::endl
            
            <<std::endl<<"[MBG]"<<std::endl;
            return STATUS_OPERATION_ERROR;
        }
        else if(marg =="--vocem" || marg == "-vocem" || marg == "--v" || marg == "-v" || marg=="vocem"){
            std::cout<< "|[ANS][VOCEM]|"<<std::endl<<"Version Code " <<VERSION_CODE << std::endl<<
            "Version Name " << VERSION_NAME << std::endl<<std::endl<<
             "|[KRR][VOCEM]|"<<std::endl<<"Version Code " <<VERSION_CODE__KRR << std::endl<<
            "Version Name " <<VERSION_NAME__KRR<< std::endl<<std::endl<<
            "More " << "https://web-mbg.ru"<<std::endl;
            return STATUS_OPERATION_ERROR;
        }
        else{
            std::string new_config = marg;
            config = read_config(new_config);
            if (config.port.length() <1 && config.index.length() <1){
                std::cerr<<"|[ANS][ERR]|[ "<< red << new_config <<reset <<" ]"<<std::endl;
                return STATUS_OPERATION_ERROR;
            }
        }
        

    }

     // =============================================================== INIT CONFIG ===============================================================

           path.href = config.href;
       path.dir_host = config.dirhost;
     path.index_file = config.index;
    path.server_port = config.port;
          path.allow = config.allow;
          path.deny = config.deny;
     path.super_buff = config.super_buff;
     path.gyper_buff = config.gyper_buff;
     path.kairrer   = config.kairrer;
    (config.content_type.length() <1 ? path.content_type = "charset=UTF-8;"+path.content_type : path.content_type = config.content_type);
    (config.server.length()       <1 ?                                    path.server="ansen" : path.server = config.server);
 

    int maxWidth = 20;
    uint16_t port;
    std::string sbf= (path.super_buff ? "true":"false");
    std::string gbf= (path.gyper_buff ? "true":"false");
    std::string krr= (path.kairrer ? "true":"false");

    std::cout << "[|] =============== [ANSEN] =============== [|] == [ "<< path.standard_name <<" ]" << std::endl;
    // std::cout << " |                href : " << std::left << std::setw(maxWidth) << (config.href.length() > maxWidth ? config.href.substr(0, maxWidth - 1) + "*" : config.href) <<                        "|" << std::endl;
    std::cout << " |                port : " << std::left << std::setw(maxWidth) << (config.port.length() > maxWidth ? config.port.substr(0, maxWidth - 1) + "*" : config.port) <<                        "|" << std::endl;
    // std::cout << " |               allow : " << std::left << std::setw(maxWidth) << (config.allow.length() > maxWidth ? config.allow.substr(0, maxWidth - 1) + "*" : config.allow) <<                     "|" << std::endl;
    // std::cout << " |                deny : " << std::left << std::setw(maxWidth) << (config.deny.length() > maxWidth ? config.deny.substr(0, maxWidth - 1) + "*" : config.deny) <<                        "|" << std::endl;
    std::cout << " |             dirhost : " << std::left << std::setw(maxWidth) << (config.dirhost.length() > maxWidth ? config.dirhost.substr(0, maxWidth - 1) + "*" : config.dirhost) <<               "|" << std::endl;
    std::cout << " |               index : " << std::left << std::setw(maxWidth) << (config.index.length() > maxWidth ? config.index.substr(0, maxWidth - 1) + "*" : config.index) <<                     "|" << std::endl;
    std::cout << " |        content-type : " << std::left << std::setw(maxWidth) << (config.content_type.length() > maxWidth ? config.content_type.substr(0, maxWidth - 1) + "*" : config.content_type)<< "|" << std::endl;
    std::cout << " |              server : " << std::left << std::setw(maxWidth) << (path.server.length() > maxWidth ? path.server.substr(0, maxWidth - 1) + "*" : path.server) <<                        "|" << std::endl;
    std::cout << " |          super-buff : " << std::left << std::setw(maxWidth) << (sbf.length() > maxWidth ? sbf.substr(0, maxWidth - 1) + "*" : sbf) <<                                                "|" << std::endl;
    std::cout << " |             kairrer : " << std::left << std::setw(maxWidth) << (sbf.length() > maxWidth ? sbf.substr(0, maxWidth - 1) + "*" : krr) <<                                                "|" << std::endl;


    
    if (config.super_buff) path.elin_ur_sutun(path.dir_host);

    // =============================================================== CREATE SOCKET ===============================================================

    sockaddr_in server_address;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    if (server_socket == STATUS_SYSTEM_OPERATION_ERROR) {
        std::cerr <<std::endl<< "|[ANS][ERR]|[FAILED CREATE SERVER]|[ "<<red<< strerror(errno) <<reset <<" ]" <<  std::endl;
        return STATUS_OPERATION_ERROR;
    }

    try{
        port = std::stoi(path.server_port);
        if (port < 1 || port > 65535){
            std::cerr<<"|[ANS][ERR]|[ "<< red << "ERR PORT"<<reset<<" ]"<<std::endl;
            return STATUS_OPERATION_ERROR;
        }
        else server_address.sin_port = htons(port);
    
    }catch(const std::exception& e){
        std::cerr << "|[ANS][ERR]|[ " <<red<<"SYNTAX PORT "<<reset<<" ]" << std::endl;
        return STATUS_OPERATION_ERROR;
    }
    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) != STATUS_OPERATION_OK) {
        std::cerr<<std::endl<<"|[ANS][ERR]|[FAILED BIND SERVER]|[ "<<red<< strerror(errno)<<reset <<" ]" << std::endl;
        return STATUS_OPERATION_ERROR;
    }
    if (listen(server_socket, SOMAXCONN) != STATUS_OPERATION_OK) {
        std::cerr<<std::endl<<"|[ANS][ERR]|[FAILED LISTEN SERVER]|[ "<<red<< strerror(errno) <<reset<<" ]" << std::endl;
        return STATUS_OPERATION_ERROR;
    }


    


    // =============================================================== START MAIN SERVER ===============================================================


    std::cout<<"\n[Y] ============== [STARTED] ============== [Y] == [ http://localhost:"<<port<<" ]\n"<<std::endl;

    


    while (true) {
        sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_len);

        if (client_socket != STATUS_SYSTEM_OPERATION_ERROR) {
            std::thread thread_client_serv(client_session, client_socket);
            thread_client_serv.detach(); 
        } else     std::cerr<<std::endl<<"|[ANS][ERR]|[FAILED CONNECION]|["<<red<< strerror(errno) <<reset<<" ]" << std::endl;
        
    }
    if (server_socket != STATUS_SYSTEM_OPERATION_ERROR)close(server_socket);
    

    return 0;
}

/*

FUNCTIONS PATHS

*/
#include <iostream>
#include <functional>

#define VERSION_CODE 18
#define VERSION_NAME "0.2"

#define STATUS_SYSTEM_OPERATION_ERROR -1
#define STATUS_OPERATION_OK 0
#define STATUS_OPERATION_ERROR 1


#define GET_DATA_OK 2
#define GET_DATA_FORBIDDEN 3
#define GET_DATA_NOTHING 4
#define GET_DATA_EL_ERROR 40

#define STR_NULL ""
bool PRAGMA_DEBUGE_VERSION = false;

int server_socket = STATUS_SYSTEM_OPERATION_ERROR;
std::string clean_string(const std::string& str);

std::string getColorStatus_code(int status_code) {
         if (status_code>=100 && status_code < 200) {return cyan;  }
    else if (status_code>=200 && status_code < 300) {return green; }
    else if (status_code>=400 && status_code < 600) {return red;   }
    else                                            {return yellow;}
}

struct request_form{
    std::string user_ip;
    std::string user_agent;
    std::time_t time;
};

struct get_form{
    std::string path;
    std::string head;
    std::string body;
    request_form user_data;
};

get_form verbos_lib(int client_socket,char* buffer){
    get_form ret_form;

    std::time_t time = std::time(0);
    std::string request(buffer);
    std::size_t pos = request.find(' ');
    std::string url = request.substr(pos + 1);
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    
    if (getpeername(client_socket, (struct sockaddr*)&client_addr, &client_addr_len) == 0) {
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, sizeof(ip_str));
        ret_form.user_data.user_ip=std::string(ip_str);   
    } 

    std::size_t headers_end = request.find("\r\n\r\n");
    if (headers_end != std::string::npos) {
        std::string headers = request.substr(0, headers_end);
        ret_form.head=headers;

        std::size_t user_agent_pos = headers.find("User-Agent:");
        if (user_agent_pos != std::string::npos) {
            std::size_t value_start = user_agent_pos + sizeof("User-Agent:") - 1; 
            std::size_t next_line_pos = headers.find('\n', value_start);          
            std::string user_agent = headers.substr(value_start, next_line_pos - value_start);
            std::string cleaned_user_agent = clean_string(user_agent);
            ret_form.user_data.user_agent=cleaned_user_agent;
        }
        
    }
    pos = url.find(' '); 
    url = url.substr(0, pos);
    std::string href_url = url.substr(url.find('/'));
    ret_form.path=href_url;
    ret_form.user_data.time = time;
    return ret_form; 
}

struct aer_func{   
    int code;
    std::vector<char> data;

};

class paths{
   
    public:
        std::string href;
        std::string dir_host;
        std::string index_file; 
        std::string server_port;
        std::string content_type; 
        std::string server;
        std::vector<std::string> allow;
        std::vector<std::string> deny;    
        std::string standard_name="mysite";
        std::string default_conf="port 5555\nindex index.html\ndirhost "+standard_name+"\ncontent-type charset=UTF-8";
        std::string default_page="<!DOCTYPE html>\n<html>\n<head>\n\t<meta charset=\"UTF-8\">\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\t<title>ANSEN</title>\n\t<style>\n\tbody {\n\t\tfont-family: Arial;\n\t\tbackground: radial-gradient(circle farthest-side at center center, white 0%, #f2f2f2 100%);\n\t\tmargin: 0;\n\t\tpadding: 0;\n\n\t}\n\tp{\n\t\tfont-size: 30px;\n\t\tmargin: 0;\n\t\tpadding: 0;\n\t}\n\tdiv{\n\t\tposition: absolute;\n\t\ttop: 1%;\n\t\tleft: 1%;\n\t\tright: 1%;\n\t\tpadding-top: 100px;\n\t\ttext-align: center;\n\t}\n\t.gin{\n\t\ttransform: scaleY(-1);\n\t\tcolor: rgb(100,100,100);\n\t}\n\ta{\n\t\tcolor: gray;\n\t}\n\tspan{\n\t\tposition: fixed;\n\t\tbottom: 100px;\n\t\tfont-size: 20px;\n\t\tmargin-left: 50px;\n\t}\n\t</style>\n</head>\n<body>\n\t<div>\n\t\t<p>Welcome to default page web server ANSEN!</p>\n\t\t<hr>\n\t\t<p class=\"gin\">Welcome to default page web server ANSEN!</p>\n\t</div>\n\t<span><a href=\"https://web-mbg.ru\">WEB-MBG</a></span>\n</body>\n</html>";
        bool super_buff;
        bool gyper_buff;
        bool kairrer;
        int sutun_aer(std::string& path, std::vector<char>& file_content) {
            std::map<std::string, std::vector<char>> file_data;
            file_data[path] = file_content;
            virtual_aer.push_back(file_data);
            return STATUS_OPERATION_OK;
        }
        
        aer_func sedel_aer(const std::string& path) {
            for (const auto& map_entry : virtual_aer) {
                if (auto it = map_entry.find(path); it != map_entry.end()) {
                    return {GET_DATA_OK, it->second};
                }
            }
            return {GET_DATA_NOTHING};
        }
        void elin_ur_sutun(std::string& root_path) {
            std::cout << "[|] ============== [BUFFER] =============== [|]" << std::endl;
            
            std::cout << std::endl << cyan << "|[ANS][SUPER-BUFF] START" << reset << std::endl;
            
            // Debug output for rules
            for (const auto& den : deny) {
                std::cout << "|[DENY]| " << den << std::endl;
            }
            for (const auto& allo : allow) {
                std::cout << "|[ALLOW]| " << allo << std::endl;
            }
        
            try {
                for (const auto& entry : std::filesystem::recursive_directory_iterator(root_path)) {
                    std::string relative_path = std::filesystem::relative(entry.path(), root_path).string();
                    std::string full_path = "/" + relative_path;
                    std::string item_name = entry.path().filename().string();
        
                    // First check if explicitly allowed (highest priority)
                    bool is_allowed = false;
                    for (const auto& allo : allow) {
                        if (allo == full_path || 
                            (allo == item_name && entry.is_regular_file()) ||
                            (full_path.find(allo + "/") == 0)) {
                            is_allowed = true;
                            break;
                        }
                    }
        
                    // If allowed, process regardless of deny rules
                    if (is_allowed) {
                        if (entry.is_regular_file()) {
                            process_file(entry, full_path, relative_path);
                        } else if (entry.is_directory()) {
                            std::cout << blue << "|[ANS][DIR-SCAN] " << full_path << reset << std::endl;
                        }
                        continue;
                    }
        
                    // Check deny rules only if not allowed
                    bool should_skip = false;
                    
                    // Check direct deny
                    for (const auto& den : deny) {
                        if (den == full_path || den == item_name) {
                            should_skip = true;
                            break;
                        }
                    }
        
                    // Check parent deny
                    if (!should_skip) {
                        std::filesystem::path parent_path = entry.path().parent_path();
                        while (!parent_path.empty() && parent_path != root_path) {
                            std::string parent_name = parent_path.filename().string();
                            for (const auto& den : deny) {
                                if (den == parent_name) {
                                    should_skip = true;
                                    break;
                                }
                            }
                            if (should_skip) break;
                            parent_path = parent_path.parent_path();
                        }
                    }
        
                    if (should_skip) {
                        std::cout << yellow << "|[ANS][FILTER] SKIP " << full_path << reset << std::endl;
                        continue;
                    }
        
                    // Default processing if not skipped
                    if (entry.is_regular_file()) {
                        process_file(entry, full_path, relative_path);
                    } else if (entry.is_directory()) {
                        std::cout << blue << "|[ANS][DIR-SCAN] " << full_path << reset << std::endl;
                    }
                }
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << red << "|[ANS][SUPER-BUFF] " << e.what() << reset << std::endl;
            }
            std::cout << cyan << "|[ANS][SUPER-BUFF] DONE" << reset << std::endl;
        }
        
        void process_file(const std::filesystem::directory_entry& entry, 
                          std::string& full_path, 
                          std::string& relative_path) {
            std::ifstream file(entry.path(), std::ios::binary | std::ios::ate);
            if (!file) {
                std::cerr << red << "|[ANS][SUPER-BUFF] DISK " << full_path << reset << std::endl;
                return;
            }
            
            auto file_size = file.tellg();
            file.seekg(0, std::ios::beg);
            
            std::vector<char> file_content(file_size);
            if (!file.read(file_content.data(), file_size)) {
                std::cerr << red << "|[ANS][SUPER-BUFF] DATA " << full_path << reset << std::endl;
                return;
            }
            
            if (sutun_aer(full_path, file_content) != STATUS_OPERATION_OK) {
                std::cerr << red << "|[ANS][SUPER-BUFF] WRITE " << relative_path << reset << std::endl;
            }
            
            std::cout << cyan << "|[ANS][SUPER-BUFF] RAM < " << full_path << reset << std::endl;
        }
    private:
        std::vector<std::map<std::string,std::vector<char>>> virtual_aer;
};
paths path;




std::string ekran_hash_sims(const std::string& path) {
    std::string sanitizedPath = path;
    size_t pos = sanitizedPath.find("..");
    while (pos != std::string::npos) {
        sanitizedPath.erase(pos, 2);
        pos = sanitizedPath.find("..");
    }
    pos = sanitizedPath.find("//");
    while (pos != std::string::npos) {
        sanitizedPath.erase(pos, 1);
        pos = sanitizedPath.find("//");
    }
    return sanitizedPath;
}
std::string clean_string(const std::string& str) {
    std::string result;
    for (char c : str) {
        if (std::isprint(c)) result.push_back(c);
    }
    return result;
}



bool file_type(const std::string& str, const std::string& suffix) {
    if (suffix.empty()) return true;
    if (suffix.size() > str.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}
std::string build_contentType(std::string&full_path,std::string&content_type) {
        std::string outContentType;
        if      (file_type(full_path, ".html") || file_type(full_path, ".htm")|| file_type(full_path, ".kr")) {outContentType = "text/html;"+content_type; }
        else if (file_type(full_path, ".css"))  {outContentType = "text/css;"+content_type;                                  }
        else if (file_type(full_path, ".js"))   {outContentType = "application/javascript;"+content_type;                    }
        else if (file_type(full_path, ".jpg") || file_type(full_path, ".jpeg")) {outContentType = "image/jpeg;"+content_type;}
        else if (file_type(full_path, ".png"))  {outContentType = "image/png;"+content_type;                                 }
        else if (file_type(full_path, ".ico"))  {outContentType = "image/x-icon;"+content_type;                              }
        else if (file_type(full_path, ".gif"))  {outContentType = "image/gif;"+content_type;                                 }
        else if (file_type(full_path, ".mp4"))  {outContentType = "video/mp4;"+content_type;                                 }
        else if (file_type(full_path, ".webm")) {outContentType = "video/webm;"+content_type;                                }
        else if (file_type(full_path, ".ogg"))  {outContentType = "audio/ogg;"+content_type;                                 }
        else if (file_type(full_path, ".pdf"))  {outContentType = "application/pdf;"+content_type;                           }    
        else{outContentType = content_type;}
        return outContentType;
}

std::string build_full_url (std::string&href_url){
    std::string full_path;

    if (path.gyper_buff == true){
        /*
        
        CODE L3 CPU GYPER BUFFER

        */
       //std::cout<<red<<"|[ANS][GYPER-BUFF] NOT SUPPORTED"<<reset<<path.gyper_buff;

    }
    
    if (path.super_buff == true){
        if (href_url == "/"){
            href_url=path.index_file;
            full_path = "/"+href_url;
        }
        else{
            full_path =  href_url;
            if (href_url.back() == '/') full_path = href_url+path.index_file;
            else { if (path.sedel_aer(full_path).code==GET_DATA_NOTHING) full_path=full_path+'/'+path.index_file; }
            
        }
        
    }
    else{

        if (href_url=="/") {
            href_url = path.index_file;
            full_path = path.dir_host + "/" + href_url;
            
        }
        else{
            full_path = path.dir_host + href_url;
            
            if (href_url.back() == '/' || is_dir(full_path)) {
                
                if (is_dir(full_path)) full_path = path.dir_host+href_url +"/" + path.index_file;
                else full_path = path.dir_host + href_url + path.index_file;
                
            }
        }
    }
    return full_path;

}

struct get_data_form{
    int code;
    std::string data;
    std::string full_url;
};
get_data_form get_data_in_url(std::string&href_url){
    
    std::string full_path=build_full_url(href_url);
    get_data_form return_data;

    if (path.gyper_buff) {}

    if (path.super_buff) {
        aer_func result = path.sedel_aer(full_path);
        return_data.code= result.code;
        return_data.data= std::string(result.data.begin(),result.data.end());
    }
    else{
        std::ifstream file(full_path);
        if( file.good() ){
            return_data.code=GET_DATA_OK;
            return_data.data=std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        }
        else{
            return_data.code=GET_DATA_NOTHING;
            return_data.data=STR_NULL;
            if (errno == EACCES) {  
                return_data.code = GET_DATA_FORBIDDEN;
                return_data.data = STR_NULL;
            } else if (errno == ENOENT || !std::filesystem::exists(full_path)) {  
                return_data.code = GET_DATA_NOTHING;
                return_data.data = STR_NULL;
            } else {
                return_data.code = GET_DATA_EL_ERROR;
                return_data.data = std::strerror(errno);
            }
        }
        
    }
    return_data.full_url=full_path;
    return return_data;
}

void zexcWer(int signal){
    if(server_socket != STATUS_SYSTEM_OPERATION_ERROR) {

        if(close(server_socket)==0){
            std::cout<<"[X] =============== [STOPED] =============== [X]"<<std::endl;
            std::exit(signal);
        }
        else std::cout<<red<<"[X] ============ [ERROR-STOPED] ============= [X]"<<reset<<std::endl;
        

    }
    
}
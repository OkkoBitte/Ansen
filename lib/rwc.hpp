/*

 READ WRITE CONFIG

*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

struct ConfigData {
    std::string href;
    std::string port; 
    std::vector<std::string> allow;
    std::vector<std::string> deny;
    std::string index;
    std::string dirhost;
    std::string content_type;
    std::string server;
    bool super_buff=false;
    bool gyper_buff=false;
    bool kairrer = false;
};

ConfigData read_config(const std::string& filename) {
    ConfigData config;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {

            std::stringstream words(line);
            std::string key, value;
            getline(words, key, ' ');
            getline(words, value, ' ');


                 if (key == "href")         config.href = value;
            else if (key == "port")         config.port = value;
            else if (key == "allow")        config.allow.push_back(value);
            else if (key == "deny")         config.deny.push_back(value);
            else if (key == "index")        config.index = value;            
            else if (key == "dirhost")      config.dirhost = value;
            else if (key == "content-type") config.content_type = value;
            else if (key == "server")       config.server = value;
            
            else if (key=="super-buff") (value=="true" ? config.super_buff=true : config.super_buff = false) ;
            else if (key=="gyper-buff") (value=="true" ? config.gyper_buff=true : config.gyper_buff = false) ;
            else if (key=="kairrer")    (value=="true" ? config.kairrer=true    : config.kairrer = false) ;
            
        }
        file.close();
    } else {
        std::cerr << "|[ASN][ERR]|[FAILED READ CONFIG][ " << filename <<" ]" << std::endl;
    }

    return config;
}


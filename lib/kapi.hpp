
#include "kairrer1/lintkrr.hpp" // KAIRRER

struct krr_respons{
    std::string head;
    std::string body;
};

krr_respons kawer(std::string code,get_form lixtform){
    krr_respons retkap;
    
    std::vector<vars> varhttp;
    
    vars kwar_path;
    kwar_path.name="requestPath";
    kwar_path.valib.type=stringV;
    kwar_path.valib.value=lixtform.path;

    vars kwar_head;
    kwar_head.name="requestHead";
    kwar_head.valib.type=stringV;
    kwar_head.valib.value=lixtform.head;

    vars kwar_body;
    kwar_body.name="requestBody";
    kwar_body.valib.type=stringV;
    kwar_body.valib.value=lixtform.body;

    vars kwar_userIP;
    kwar_userIP.name="requestUserIP";
    kwar_userIP.valib.type=stringV;
    kwar_userIP.valib.value=lixtform.user_data.user_ip;

    vars kwar_userAgent;
    kwar_userAgent.name="requestUserAgent";
    kwar_userAgent.valib.type=stringV;
    kwar_userAgent.valib.value=lixtform.user_data.user_agent;

    varhttp.push_back(kwar_path);
    varhttp.push_back(kwar_head);
    varhttp.push_back(kwar_body);

    varhttp.push_back(kwar_userIP);
    varhttp.push_back(kwar_userAgent);

    std::vector<vars> retvars=kairrer(code,varhttp);
    
    retkap.head=NULL_STR;
    retkap.body=NULL_STR;


    for(vars var:retvars){
        if(var.name=="head") retkap.head=var.valib.value;
        if(var.name=="body") retkap.body=var.valib.value;
    }
    
    return retkap;
}
#include "bed.hpp"
std::vector<vars> kairrer(std::string code,std::vector<vars> argm){
    std::vector<TOKENS> tokens = tokenizer(code);
    urwerer state;
    kairrer_tpr ktpr(tokens, state);

    vars AP;
    AP.name = "AP";
    AP.valib.type = VSID::intV;
    AP.valib.value = "1";
    argm.push_back(AP);

    vars BIN;
    BIN.name = "BIN";
    BIN.valib.type = VSID::intV;
    BIN.valib.value = "0";
    argm.push_back(BIN);

    vars FO;
    FO.name = "FO";
    FO.valib.type = VSID::intV;
    FO.valib.value = "-1";
    argm.push_back(FO);

    return  ktpr.prun(argm);
}
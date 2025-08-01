class kairrer_tpr {
    std::vector<TOKENS> tokens;
    urwerer& state;
    urwerer::ASM asmcode;
    urwerer::HexExecutor hexcode;
    size_t vap = 0;
    LOG log;

    const TOKENS& lift() { return tokens[vap--]; } 
    const TOKENS& cret() { return tokens[vap++]; }
    const TOKENS& lixt() const { return tokens[vap]; } 
    TOKENS& lixtCret(int cret) {
        int newpin = vap;
        return tokens[newpin+cret];
    } 
    bool vaZexc() const { return vap >= tokens.size(); } 
    bool vadap(TSID type) const { return !vaZexc() && lixt().type == type; }
    bool dapin(TSID type) { 
        if (vadap(type)) { cret(); return true; }
        return false;
    }
    bool daplift(TSID type){ 
        if (vadap(type)) return true; 
        
        lift();
        return false;
    }
    bool esValue(){
        return vadap(INT) || vadap(SI) || vadap(STRING) || vadap(S) || vadap(SS) || vadap(GE);
    }

    void skipTAB(){while(vadap(TSID::TAB))cret();} 
    void skipLINE(){while(!dapin(TSID::INGOR) &&!vaZexc())cret();}
    void echoLIXT(){ std::cout<<"'"<<lixt().value<<"'"<<std::endl;} 
    int  getLINE(){
        int newpin = vap;
        int lines = 1;
        while(newpin != 0){
            if(tokens[newpin].type==TSID::INGOR) lines++;
            newpin--;
        }
        return lines;
    }
    int getSINE() {
        int newpin = vap;
        int sims = 0;
        while (newpin != 0 && tokens[newpin].type!=INGOR){
            sims ++;
            newpin -- ;
        }
        return sims;
    }
    std::string line_pin(){return "["+std::to_string(getLINE())+"|"+std::to_string(getSINE())+"]";}

    std::string lixtS() {
        std::string value;
        cret();
        while (!vaZexc() && !vadap(TSID::S)) {
        
            
            if (dapin(TSID::LINK)) {
                vars varlink= lixtLINK();
                if(varlink.valib.type!=VSID::stringV&&varlink.valib.type!=VSID::intV) log.fkr("S","gat type var ' <- et "+line_pin());
                value += varlink.valib.value; 
            }
            else value += cret().value;
        };
        if (!dapin(TSID::S)) log.fkr("S", "un drit et -> " + SIMS::get_sname_from_name(TSID::S)+line_pin());
        return value;
    }
    std::string lixtSS() { 
        std::string value;
        cret();
        while (!vaZexc() && !vadap(TSID::SS)) {
            if (dapin(TSID::LINK)) {
                vars varlink= lixtLINK();
                if(varlink.valib.type!=VSID::stringV&&varlink.valib.type!=VSID::intV) log.fkr("SS","gat type var \" <- et "+line_pin());
                value += varlink.valib.value; 
            }
            else value += cret().value;
        }
        if (!dapin(TSID::SS)) log.fkr("SS","un drit et -> " + SIMS::get_sname_from_name(TSID::SS)+line_pin());
        return value;
    }
    std::string lixtGG() {
        std::string value;
        int nest_level = 1; 
        cret(); 
        
        while (!vaZexc() && nest_level > 0) {
            if (vadap(TSID::GE)) { 
                nest_level++;
                value += cret().value;
            } 
            else if (vadap(TSID::GU)) {
                nest_level--;
                if (nest_level > 0) value += cret().value;
            }
            else {
                value += cret().value;
            }
        }
        
        if (nest_level != 0) {
            log.fkr("GG","li dreban ant -> } "+line_pin());
        }
        
        return value;
    }
    vars        lixtLINK(){
        vars retvar;
        std::string varname =lixtSTRINT();
        auto var = state.getVariable(varname);
        if (var){
                
            retvar.name=varname;
            retvar.valib.type=var->valib.type;
            retvar.valib.value=var->valib.value;
        } 
        else  log.fkr("LINK","fo vanih et var: @" + varname + line_pin());
        return retvar;
    }
    std::string lixtINT(){
        std::string senInt;
        if (dapin(TSID::SI)) senInt+=SIMS::get_sim_from_name(TSID::SI);
        while(!vaZexc() && vadap(TSID::INT))senInt+=cret().value;
        
        return senInt;
    }
    std::string lixtSTRING() {
        std::string stringData;
        while (vap < tokens.size() && vadap(TSID::STRING))stringData+= cret().value;    
        return stringData;
    }
    std::string lixtSTRINT(){
        std::string stringData;
        while (vap < tokens.size() && (vadap(TSID::STRING) ||vadap(TSID::INT)))stringData+= cret().value;    
        return stringData;
    }
    value       lixtVALUE(){
        value vlib;
        if(vadap(TSID::S)){
            vlib.type=VSID::stringV;
            vlib.value=lixtS();
        }
        else if(vadap(TSID::SS)){
            vlib.type=VSID::stringV;
            vlib.value=lixtSS();
        }
        else if(vadap(TSID::GE)){
            vlib.type=VSID::runV;
            vlib.value=lixtGG();
        }
        
        else if( vadap(TSID::INT) ||  vadap(TSID::SI) ){
            vlib.type=VSID::intV;
            // 0x
            vlib.value=lixtINT();
        }

        else if (vadap(TSID::STRING))  vlib=varWer().valib;
        
        else if (dapin(TSID::DIG) && lift().type==TSID::DIG) {
            vlib.type=VSID::stringV;
            vlib.value=state.executeCommand(lixtShell());
        }
        else if (dapin(TSID::ZIG) && lift().type==TSID::ZIG) {
            vlib.type=VSID::intV;
            vlib.value=std::to_string(asmcode.executeASM(lixtAsm()));
        }

        else log.fkr("VALUE","fat ret ame value et -> '"+lixt().value+"' "+line_pin());
        return vlib;
    }
    
    
    std::string lixtShell() {
        cret();cret();
    
        std::string command;
        bool foundClosing = false;
    
        while (!vaZexc()) {
            if (dapin(TSID::DIG)) {
                size_t pos = vap; 
                if (!vaZexc() && dapin(TSID::DIG)) {
                    foundClosing = true;
                    break;
                } else {
                    vap = pos;
                    command += '_';
                }
            }
            else if (dapin(TSID::LINK)) {
                vars varlink= lixtLINK();
                if(varlink.valib.type!=VSID::stringV&&varlink.valib.type!=VSID::intV) log.fkr("SHELL","gat ret var: "+get_sname_from_vname(varlink.valib.type)+" kamun sidact: "+varlink.name+line_pin());
                
                command += varlink.valib.value; 
            }
            else command += cret().value;
        
        }
    
        if (!foundClosing) log.fkr("SHELL","un drit et -> '__' per zexc "+line_pin());
            
        
        return command;
    }
    std::string lixtAsm(){

        cret();cret();
    
        std::string command;
        bool foundClosing = false;
    
        while (!vaZexc()) {
            if (dapin(TSID::ZIG)) {
                size_t pos = vap; 
                if (!vaZexc() && dapin(TSID::ZIG)) {
                    foundClosing = true;
                    break;
                } else {
                    vap = pos;
                    command += '*';
                }
            }
            else if (dapin(TSID::LINK)) {
                vars varlink= lixtLINK();
                if(varlink.valib.type!=VSID::stringV&&varlink.valib.type!=VSID::intV) log.fkr("ASM","gat ret var: "+get_sname_from_vname(varlink.valib.type)+" kamun sidant: "+varlink.name+line_pin());
                command += varlink.valib.value; 
            }
            else command += cret().value;
            
        }
    
        if (!foundClosing) log.fkr("ASM","un drit et -> '**' per zexc "+line_pin());
            
        
        return command;
    }
    std::string lixtHex(){
        cret();cret();
        std::string command;
        bool foundClosing = false;
    
        while (!vaZexc()) {
            if (dapin(TSID::TAK)) {
                size_t pos = vap; 
                if (!vaZexc() && dapin(TSID::TAK)) {
                    foundClosing = true;
                    break;
                } else {
                    vap = pos;
                    command += '.';
                }
            }
            else if (dapin(TSID::LINK)) {
                vars varlink= lixtLINK();
                if(varlink.valib.type!=VSID::stringV&&varlink.valib.type!=VSID::intV) log.fkr("HER","gat ret var: "+get_sname_from_vname(varlink.valib.type)+" kamun sidact: "+varlink.name+line_pin());
                command += varlink.valib.value; 
            }
            else command += cret().value;

        }
    
        if (!foundClosing) log.fkr("HER","un drit et -> '..' per zexc "+line_pin());
        return command;
        
    }
    

    vars        reVarWer (vars donner, vars akcepter){
        // akcepter < donner ;
        
        //std::cout<<"D:"<<donner.name<<" > A:"<<akcepter.name<<std::endl;

        VSID donner_type = donner.valib.type;
        VSID akcepter_type = akcepter.valib.type;
        
        if(akcepter_type == runV) {
        
            akcepter.valib.rargs.push_back( donner );
            
            if(donner_type==nullV){
                log.fkr("RESUTUN","fatneir et sutun ret nullV ot runV " +line_pin());
            }
        }

        else if(akcepter_type == stringV){
            if (donner_type == stringV){
                akcepter.valib.value = donner.valib.value;
            }


            else if(donner_type == intV) {
                int descis;
                try { 
                    descis = atoi(donner.valib.value.c_str());
                } catch(...) {
                    log.fkr("RESUTUN", "var intV fo eplis intV "+line_pin());
                    return akcepter;
                }
                if (is_valid_descriptor(descis)) {
                    int fd = descis;
                    
                  
                    struct stat file_stat;
                    if(fstat(fd, &file_stat) == -1) {
                        log.fkr("RESUTUN", "fo wer et lixt lib liber "+line_pin());
                        return akcepter;
                    }
                    off_t file_size = file_stat.st_size;
                    
                  
                    std::vector<char> buffer(file_size + 1);
                    
                  
                    ssize_t bytes_read = read(fd, buffer.data(), file_size);
                    if(bytes_read == -1) {
                        log.fkr("RESUTUN", "fo wer et sedel liber "+line_pin());
                        return akcepter;
                    }
                    
                  
                    buffer[bytes_read] = '\0'; 
                    akcepter.valib.type = stringV;
                    akcepter.valib.value = std::string(buffer.data());
                    
                  
                    //std::cout << "Read " << bytes_read << " bytes from fd " << fd <<"And value"<<akcepter.valib.value<< std::endl;
                }
                else {
                    akcepter.valib.value = donner.valib.value;
                }
            }


            else if(donner_type == runV){
                log.fkr("RESUTUN","fatneir et sutun var ret runV ot stringV "+line_pin());

            }
            else if(donner_type == nullV){
                log.cev("stringV < nullV; fater wer;");
            }
        }
        else if(akcepter_type == intV){
            if (donner_type == stringV){
                int descis;
                try{ descis = atoi(akcepter.valib.value.c_str());}catch(...){
                    log.fkr("RESUTUN","var intV fo eplis intV "+line_pin());
                }
                if (is_valid_descriptor(descis)){
                    int fd = atoi(akcepter.valib.value.c_str());
                    akcepter.valib.type=intV;
                    akcepter.valib.value=std::to_string(write(fd, donner.valib.value.c_str(), donner.valib.value.size()));
                    //std::cout<<"<w>{ desc:"<<fd<<"; value:"<<donner.valib.value.c_str()<<"; size:"<<donner.valib.value.size()<<"; }"<<std::endl;
                }
                else log.fkr("RESUTUN","fatneir et sutun var ret stringV ot intV "+line_pin());
            }
            else if(donner_type == intV){
                akcepter.valib.value = donner.valib.value;
            }
            else if(donner_type == runV){
                log.fkr("RESUTUN","fatneir et sutun var ret runV ot stringV "+line_pin());

            }
            else if(donner_type == nullV){
                log.cev("stringV < nullV; fater wer; "+line_pin());
            }
        }

        else if(akcepter_type == nullV){
            akcepter.valib.type = donner.valib.type;
            akcepter.valib.value = donner.valib.value;
            if(donner_type==intV){
                int descis;
                try { 
                    descis = atoi(donner.valib.value.c_str());
                } catch(...) {
                    log.fkr("RESUTUN", "var intV fo eplis intV "+line_pin());
                    return akcepter;
                }
                if (is_valid_descriptor(descis)) {
                    int fd = descis;
                    
                  
                    struct stat file_stat;
                    if(fstat(fd, &file_stat) == -1) {
                        log.fkr("RESUTUN", "fo wer et lixt lib liber "+line_pin());
                        return akcepter;
                    }
                    off_t file_size = file_stat.st_size;
                    
                  
                    std::vector<char> buffer(file_size + 1);
                    
                  
                    ssize_t bytes_read = read(fd, buffer.data(), file_size);
                    if(bytes_read == -1) {
                        log.fkr("RESUTUN", "fo wer et sedel liber "+line_pin());
                        return akcepter;
                    }
                    
                  
                    buffer[bytes_read] = '\0'; 
                    akcepter.valib.type = stringV;
                    akcepter.valib.value = std::string(buffer.data());
                    
                  
                    //std::cout << "Read " << bytes_read << " bytes from fd " << fd <<"And value"<<akcepter.valib.value<< std::endl;
                }
                else {
                    akcepter.valib.value = donner.valib.value;
                }

            }
        }
        
        if(akcepter.valib.type!=runV) state.addVariable(akcepter);
        return akcepter;
    }
    vars        reVarBos(vars veif, vars vurf, __vawer vawer) {
        vars retvar;
        retvar.valib.type = nullV; 
        retvar.valib.value = NULL_STR;
    
        VSID er = veif.valib.type;
        VSID ur = vurf.valib.type;
        
             if (vawer == CU_SPI) { // +> 
                
                 if (er == nullV || ur == nullV) {
                
                retvar = (ur == nullV) ? veif : vurf;
            }
            else if (er == intV && ur == intV) {
                int result = atoi(veif.valib.value.c_str()) + atoi(vurf.valib.value.c_str());
                retvar.valib.type = intV;
                retvar.valib.value = std::to_string(result);
            }
            else if (er == intV && ur == stringV) {
                retvar.valib.type = stringV;
                retvar.valib.value = vurf.valib.value + veif.valib.value;
            }
            else if (er == stringV && ur == intV) { 
                retvar.valib.type = stringV;
                retvar.valib.value = veif.valib.value + vurf.valib.value;
            }
            else if (er == stringV && ur == stringV) {
                retvar.valib.type = stringV;
                retvar.valib.value = vurf.valib.value + veif.valib.value;
            }
            else if (er == intV && ur == hexV) {
                int veifInt = stoi(veif.valib.value);
                int vurfInt = stoi(vurf.valib.value, 0, 16);
                int result = veifInt + vurfInt;
                retvar.valib.type = hexV;
                retvar.valib.value = "0x" + std::to_string(result);
            }
            else if (er == hexV && ur == intV) {
                int veifInt = stoi(veif.valib.value, 0, 16);
                int vurfInt = stoi(vurf.valib.value);
                int result = veifInt + vurfInt;
                retvar.valib.type = hexV;
                retvar.valib.value = "0x" + std::to_string(result);
            }
            else if (er == hexV && ur == hexV) {
                int veifInt = stoi(veif.valib.value, 0, 16);
                int vurfInt = stoi(vurf.valib.value, 0, 16);
                int result = veifInt + vurfInt;
                retvar.valib.type = hexV;
                retvar.valib.value = "0x" + std::to_string(result);
            }
            else if (ur == runV) {
                log.fkr("REVERBOS","fatneir wer <+,+>  ten ret 'runV' "+line_pin());
            }
            else if (er == runV) {
                log.fkr("REVERBOS","fatneir wer <+,+>  ten ret <runV> "+line_pin());    
            }
        }
        else if (vawer == CE_SPI) { // <+
            
                 if (er == nullV || ur == nullV) {
                retvar = (er == nullV) ? vurf : veif;
            }
            else if (er == intV && ur == intV) {
                int result = atoi(veif.valib.value.c_str()) + atoi(vurf.valib.value.c_str());
                retvar.valib.type = intV;
                retvar.valib.value = std::to_string(result);
            }
            else if (er == intV && ur == stringV) { // 
                retvar.valib.type = stringV;
                retvar.valib.value = veif.valib.value + vurf.valib.value;
            }
            else if (er == stringV && ur == intV) {
                retvar.valib.type = stringV;
                retvar.valib.value = veif.valib.value + vurf.valib.value;
            }
            else if (er == stringV && ur == stringV) {
                retvar.valib.type = stringV;
                retvar.valib.value = veif.valib.value + vurf.valib.value;
            }
            else if (er == intV && ur == hexV) {
                int veifInt = stoi(veif.valib.value);
                int vurfInt = stoi(vurf.valib.value, 0, 16);
                int result = veifInt + vurfInt;
                retvar.valib.type = intV; 
                retvar.valib.value = std::to_string(result);
            }
            else if (er == hexV && ur == intV) {
                int veifInt = stoi(veif.valib.value, 0, 16);
                int vurfInt = stoi(vurf.valib.value);
                int result = veifInt + vurfInt;
                retvar.valib.type = hexV;
                retvar.valib.value = "0x" + std::to_string(result);
            }
            else if (er == hexV && ur == hexV) {
                int veifInt = stoi(veif.valib.value, 0, 16);
                int vurfInt = stoi(vurf.valib.value, 0, 16);
                int result = veifInt + vurfInt;
                retvar.valib.type = hexV;
                retvar.valib.value = "0x" + std::to_string(result);
            }
            else if (er == runV) {
                log.fkr("REVERBOS","fatneir wer <+,+>  ten ret 'runV' "+line_pin());
            }
            else if (ur == runV) {
                log.fkr("REVERBOS","fatneir wer <+,+>  ten ret 'runV' "+line_pin());
            }
        }
    
        else if(vawer == CU_SI) {// ->
            auto envarlib=state.getVariable(veif.name);
            if (!envarlib) log.fkr("REWERBOS","fo vanih et var '"+veif.name+"' ; ten wer '->' "+line_pin());
            std::string vanihetvarname = vurf.name;
            bool vavanih=false;
            for(auto varg : envarlib->valib.rargs){
                if(varg.name==vanihetvarname){

                    retvar.valib=varg.valib;
                    vavanih=true;
                    
                }
            }
            if(!vavanih) log.fkr("REWERBOS","fovanih et var '"+vanihetvarname+"' ; ten wer '->'  ut var '"+veif.name+"'"+line_pin());
            
        }
        else if(vawer == CE_SI) {// <-
           
            auto envarlib=state.getVariable(vurf.name);
            if (!envarlib) log.fkr("REWERBOS","fo vanih et var '"+vurf.name+"' ; ten wer '->' "+line_pin());
            std::string vanihetvarname = veif.name;
            bool vavanih=false;
            for(auto varg : envarlib->valib.rargs){
                if(varg.name==vanihetvarname){

                    retvar.valib=varg.valib;
                    vavanih=true;
                    
                }
            }
            if(!vavanih) log.fkr("REWERBOS","fovanih et var '"+vanihetvarname+"' ; ten wer '->'  ut var '"+vurf.name+"'"+line_pin());
        }
        
        state.addVariable(retvar);
        return retvar;
    }

    vars        varWer(){
        vars varlib;
        std::string vname = lixtSTRINT();
        varlib.name=vname;
        auto verZIG = [&](vars& varlib, const std::string& vname, bool saveToState = true) {
            cret();
            bool vasavemy = false;
            auto vanih = state.getVariable(vname);
            if (vanih) {
                if (vanih->valib.type == runV) {
  
                    if (dapin(ZIG)) vasavemy = true;
                    std::string virtualcode = vanih->valib.value;
                    std::vector<TOKENS> tokensrunvs = tokenizer(virtualcode);
                    urwerer cp_state = state;
                    kairrer_tpr ktpr(tokensrunvs, cp_state);
    
                    varlib.valib.type = runV;
                    varlib.valib.rargs = ktpr.prun(vanih->valib.rargs);
                    

                    // for (auto d:vanih->valib.rargs){
                    //     std::cout<<"["<<d.name<<"]|["<<d.valib.value<<"]:"<<get_sname_from_vname(d.valib.type)<<std::endl;
                    // }

                    for (auto retvar : varlib.valib.rargs) {
                        if (retvar.name == defretvarname) {
                            varlib.valib.type = retvar.valib.type;
                            varlib.valib.value = retvar.valib.value;
                        }
                    }
    
                    if (vasavemy && saveToState) state.addVariable(varlib);
                }
                else log.fkr("ZIG", "var: " + vname + " <- ut fat run, fal ret "+line_pin());
            }
            else log.fkr("ZIG", "var: " + vname + " <- ut fat eplis "+line_pin());
        };

        if(vadap(ZIG)) {verZIG(varlib, vname); return varlib;}
        
        skipTAB();


        /* : */if     (vadap(BEDTAK)){
            cret();
            skipTAB();
            
            varlib.valib = lixtVALUE();
            state.addVariable(varlib);
        }
        

        /* <,+,-; -; +;*/  else if( (vadap(CE) && (lixtCret(1).type==SPI || lixtCret(1).type==SI)) || ((vadap(SI) || vadap(SPI)) && lixtCret(1).type==CU) ){ //  && (lift().type==SPI || lixt().type==SI )|| dapin(SI)  || vadap(SPI)
            // VAR ACTIONS 

            __vawer vawer;
            auto givKair = [&](){ log.fkr("REWERBOS","gat sim ten wer: <+, <- vel +> ->; gat sim et -> "+lixt().value +line_pin()); };
            if (vadap(CE)){
                     if(lixtCret (1).type==SPI ) vawer = __vawer::CE_SPI;  // <+
                else if(lixtCret (1).type==SI  ) vawer = __vawer::CE_SI;   // <- 
                else givKair();
            }
            else if(vadap(SPI) && lixtCret (1).type==CU) vawer =__vawer::CU_SPI; // +>
            else if(vadap(SI)  && lixtCret (1).type==CU) vawer =__vawer::CU_SI;  // -> 
            
            else givKair();
            cret();cret();
            skipTAB();
            // VAR TO VAR
            /* urf-var */   if(vadap(STRING)){
                std::string rvname = lixtSTRINT();
                
                auto veif = state.getVariable(vname);

                vars vareif;
                if(veif){
                    vareif.name = veif->name;
                    vareif.valib= veif->valib;
                }else{
                    vareif.name = vname;
                    vareif.valib.type = VSID::nullV;
                    vareif.valib.value= NULL_STR;
                }
                auto vurf = state.getVariable(rvname);

                vars varurf;
                if(vurf){
                    varurf.name =vurf->name;
                    varurf.valib=vurf->valib;
                }else{
                    varurf.name = rvname;
                    varurf.valib.type = VSID::nullV;
                    varurf.valib.value= NULL_STR;
                }

                varlib.valib = reVarBos(vareif, varurf, vawer).valib;
            }
            // VAR TO VALUE
            /* urf-value */ else if(esValue()){
                auto es_eifvar = state.getVariable(vname);
                
                vars eifvar;
                eifvar.name = vname;

                if (es_eifvar)  eifvar.valib = es_eifvar->valib;
                else{
                    eifvar.valib.type  = nullV;
                    eifvar.valib.va_ap = NULL_STR; 
                } 

                value urfvalue = lixtVALUE();

                VSID er = eifvar.valib.type;
                VSID ur = urfvalue.type;
                std::string evalue = eifvar.valib.value;
                std::string uvalue = urfvalue.value;
                     if (vawer == CU_SPI) { // +> 
                    if (er == nullV || ur == nullV) varlib.valib = (ur == nullV) ? eifvar.valib : urfvalue;
                    else if (er == intV){
                        if(ur == intV){
                            int result = atoi(evalue.c_str()) + atoi(uvalue.c_str());
                            varlib.valib.type = intV;
                            varlib.valib.value = std::to_string(result);
                        }
                        else if (ur == stringV){
                            varlib.valib.type = stringV;
                            varlib.valib.value = evalue+uvalue;
                        }    
                        else if (ur == runV){
                            log.fkr("REVERBOS","fatneir reverbos ret <intV> et <runV>");
                        }
    
                    }
                    else if (er == stringV){
                        if (ur == intV){
                            varlib.valib.type  = stringV;
                            varlib.valib.value = evalue+uvalue;
                        }        
                        else if (ur == stringV){
                            varlib.valib.type = stringV;
                            varlib.valib.value = evalue+uvalue;
                        }    
                        else if (ur == runV){
                            log.fkr("REVERBOS","fatneir reverbos ret <intV> et <runV>");
                        }
                         
                    }
                    else if (er == runV){
                        log.fkr("REVERBOS","fatneir reverbos ret <runV> ten wer VALUE");
                    }
                    
                    
                }
                else if (vawer == CE_SPI) { // <+
                    if (er == nullV || ur == nullV) varlib.valib = (ur == nullV) ? eifvar.valib : urfvalue;
                    else if (er == intV){
                        if(ur == intV){
                            int result = atoi(evalue.c_str()) + atoi(uvalue.c_str());
                            varlib.valib.type = intV;
                            varlib.valib.value = std::to_string(result);
                        }
                        else if (ur == stringV){
                            varlib.valib.type = stringV;
                            varlib.valib.value = evalue+uvalue;
                        }    
                        else if (ur == runV){
                            log.fkr("REVERBOS","fatneir reverbos ret <intV> et <runV>");
                        }
    
                    }
                    else if (er == stringV){
                        if (ur == intV){
                            varlib.valib.type  = stringV;
                            varlib.valib.value = evalue+uvalue;
                           
                        }        
                        else if (ur == stringV){
                            varlib.valib.type = stringV;
                            varlib.valib.value = evalue+uvalue;
                        }    
                        else if (ur == runV){
                            log.fkr("REVERBOS","fatneir reverbos ret <intV> et <runV>");
                        }
                         
                    }
                    else if (er == runV){
                         log.fkr("REVERBOS","fatneir reverbos ret <runV>");
                    }
                    
                    
                }
                
                else if(vawer == CU_SI) {   // ->
                    if (er == nullV){
                        log.fkr("REWERBOS","fatneir lixt lib ut var ret <nullV>");
                    }
                    else if (er == intV){
                        if (ur == intV){
                            int result = atoi(evalue.c_str()) - atoi(uvalue.c_str());
                            varlib.valib.type = intV;
                            varlib.valib.value = std::to_string(result);
                        }
                        else if (ur == stringV){
                            log.fkr("REWERBOS","fatneir lixt lib et var ret <intV> ut var ret <stringV>");
                        }
                        else if (ur == runV){
                            log.fkr("REWERBOS","fatneir lixt lib et var ret <intV> ut var ret <runV>");
                        }
                    }
                    else if (er == stringV){
                        if (ur == intV){
                            int index = std::stoi(uvalue);
                            std::string result;
                            if (index < 0 || index >= evalue.length()) log.fkr("REWERBOS","fat lixten sim ut var '"+eifvar.name+"' et sim "+ uvalue);
                            if (index == 0)result = evalue;
                            else result = evalue[evalue.length() - index]; 
                            varlib.valib.type = stringV;
                            varlib.valib.value = result;
                        }
                        else if (ur == stringV){
                            std::string result;
                            size_t pos = evalue.find(uvalue);
                            if (pos != std::string::npos) result = evalue.erase(pos, uvalue.length());
                            else result = evalue;
                            varlib.valib.type = stringV;
                            varlib.valib.value = result;
                        }
                        else if (ur == runV){
                            log.fkr("REWERBOS","fatneir lixt lib et var ret <intV> ut var ret <runV>");
                        }
                    }
                    else if (er == runV){
                        log.fkr("REVERBOS","fatneir reverbos ret <runV> ten wer VALUE");
                    }
                }
              else if(vawer == CE_SI) { // <-
                if (er == nullV) {
                    varlib.valib.type = nullV;
                    varlib.valib.value = NULL_STR;
                }
                else if (er == intV) {
                    if (ur == intV) {
                        
                        int result = atoi(uvalue.c_str()) - atoi(evalue.c_str());
                        varlib.valib.type = intV;
                        varlib.valib.value = std::to_string(result);
                    }
                    else if (ur == stringV) {
                        int index = std::stoi(evalue);
                        std::string result;
                        if (index <= 0 || index > uvalue.length()) log.fkr("REWERBOS","fat lixten sim ut VALUE '"+uvalue+"' et var "+ eifvar.name);
                        if (index == 0)result = uvalue;
                        else result=uvalue[index-1];
                        varlib.valib.type = stringV;
                        varlib.valib.value = result;
                      
                    }
                    else if (ur == runV) {
                        log.fkr("REWERBOS","fatneir lixt lib et var ret <intV> ut var ret <runV>");
                    }
                }
                else if (er == stringV) {
                    if (ur == intV) {
                        log.fkr("REWERBOS","fatneir lixt lib et var ret <stringV> ut var ret <intV>");
                    }
                    else if (ur == stringV) {
                        size_t pos = evalue.rfind(uvalue);
                        std::string result = evalue;
                        if (pos != std::string::npos) {
                            result.erase(pos, uvalue.length());
                        }
                        varlib.valib.type = stringV;
                        varlib.valib.value = result;
                    }
                    else if (ur == runV) {
                        log.fkr("REWERBOS","fatneir lixt lib et var ret <stringV> ut var ret <runV>");
                    }
                }
                else if (er == runV) {
                    log.fkr("REVERBOS","fatneir reverbos ret <runV> ten wer VALUE");
                }
            }

            }
        }

        /* > ; < */   else if ( vadap(CU) || vadap(CE) ){
            // REWRITE VAR TO VAR
            bool revers = false;
            if(vadap(CE)) revers = true;
            cret();

            
            auto envarlib=state.getVariable(vname);
            if (!envarlib) log.fkr("CE|CU","fo vanih et var '"+vname+"' "+line_pin());
            
            vars donner;
            donner.name=envarlib->name;
            donner.valib=envarlib->valib;
            
            skipTAB();
            
            if(vadap(STRING)){
                vars akcepter  = varWer();
                varlib.valib   = reVarWer( ( revers ? akcepter : donner ) , ( revers ? donner : akcepter ) ).valib; 
                skipTAB();
                if (vadap(ZIG)) verZIG(varlib, vname);
            }
            else if(vadap(DREB)) state.removeVariable(vname);

            else{
                // RIGHT DATA IS TYPE VALUE
                if(revers){
                    value donor_value=lixtVALUE();
                    auto akcepter_type = donner.valib.type; // inper kamen akceptor
                    auto    donner_type = donor_value.type;
                    vars akcepter=donner;
                    if(akcepter_type == runV) {
        
                        log.fkr("RESUTUN","fatneir et sutun ret fat-var ot runV "+line_pin());
                    }
                    else if(akcepter_type == stringV){
                        if (donner_type == stringV){
                            akcepter.valib.value = donor_value.value;
                        }
            
            
                        else if(donner_type == intV) {
                            int descis;
                            try { 
                                descis = atoi(donor_value.value.c_str());
                            } catch(...) {
                                log.fkr("RESUTUN", "var intV fo eplis intV " +line_pin());
                                return akcepter;
                            }
                            if (is_valid_descriptor(descis)) {
                                int fd = descis;
                                
                              
                                struct stat file_stat;
                                if(fstat(fd, &file_stat) == -1) {
                                    log.fkr("RESUTUN", "fo wer et lixt lib liber "+line_pin());
                                    return akcepter;
                                }
                                off_t file_size = file_stat.st_size;
                                
                              
                                std::vector<char> buffer(file_size + 1);
                                
                              
                                ssize_t bytes_read = read(fd, buffer.data(), file_size);
                                if(bytes_read == -1) {
                                    log.fkr("RESUTUN", "fo wer et sedel liber "+line_pin());
                                    return akcepter;
                                }
                                
                              
                                buffer[bytes_read] = '\0'; 
                                akcepter.valib.type = stringV;
                                akcepter.valib.value = std::string(buffer.data());
                                
                              
                                //std::cout << "Read " << bytes_read << " bytes from fd " << fd <<"And value"<<akcepter.valib.value<< std::endl;
                            }
                            else {
                                akcepter.valib.value = donor_value.value;
                            }
                        }
            
            
                        else if(donner_type == runV){
                            log.fkr("RESUTUN","fatneir et sutun var ret runV ot stringV "+line_pin());
            
                        }
                        else if(donner_type == nullV){
                            log.cev("stringV < nullV; fater wer "+line_pin());
                        }
                    }
                    else if(akcepter_type == intV){
                        if (donner_type == stringV){
                            int descis;
                            try{ descis = atoi(akcepter.valib.value.c_str());}catch(...){
                                log.fkr("RESUTUN","var intV fo eplis intV "+line_pin());
                            }
                            if (is_valid_descriptor(descis)){
                                //int fd = atoi(akcepter.valib.value.c_str());
                                akcepter.valib.type=intV;
                                akcepter.valib.value=std::to_string(write(descis, donor_value.value.c_str(), donor_value.value.size()));
                                //std::cout<<"<v>{ desc:"<<descis<<"; value:"<<donor_value.value.c_str()<<"; size:"<<donor_value.value.size()<<"; }"<<std::endl;
                            }
                            else log.fkr("RESUTUN","fatneir et sutun var ret stringV ot intV "+line_pin());
                        }
                        else if(donner_type == intV){
                            akcepter.valib.value = donor_value.value;
                        }
                        else if(donner_type == runV){
                            log.fkr("RESUTUN","fatneir et sutun var ret runV ot stringV "+line_pin());
            
                        }
                        else if(donner_type == nullV){
                            log.cev("stringV < nullV; fater wer "+line_pin());
                        }
                    }
            
                    else if(akcepter_type == nullV){
                        akcepter.valib.type = donor_value.type;
                        akcepter.valib.value = donor_value.value;
                        if(donner_type == intV) {
                            int descis;
                            try { 
                                descis = atoi(donor_value.value.c_str());
                            } catch(...) {
                                log.fkr("RESUTUN", "var intV fo eplis intV "+line_pin());
                                return akcepter;
                            }
                            if (is_valid_descriptor(descis)) {
                                int fd = descis;
                                
                              
                                struct stat file_stat;
                                if(fstat(fd, &file_stat) == -1) {
                                    log.fkr("RESUTUN", "fo wer et lixt lib liber "+line_pin());
                                    return akcepter;
                                }
                                off_t file_size = file_stat.st_size;
                                
                              
                                std::vector<char> buffer(file_size + 1);
                                
                              
                                ssize_t bytes_read = read(fd, buffer.data(), file_size);
                                if(bytes_read == -1) {
                                    log.fkr("RESUTUN", "fo wer et sedel liber "+line_pin());
                                    return akcepter;
                                }
                                
                              
                                buffer[bytes_read] = '\0'; 
                                akcepter.valib.type = stringV;
                                akcepter.valib.value = std::string(buffer.data());
                                
                              
                               // std::cout << "Read " << bytes_read << " bytes from fd " << fd <<"And value"<<akcepter.valib.value<< std::endl;
                            }
                            else {
                                akcepter.valib.value = donor_value.value;
                            }
                        }


                    }
                    
                    if(akcepter.valib.type!=runV) state.addVariable(akcepter);

                    varlib = akcepter;  
                }
                else log.fkr("CE|CU","fat wer re-sutun et sim -> '"+lixt().value+"' "+line_pin());
            }
            

        }
       
        /* -; 0; '; " */ else if (vadap(SI) || vadap(INT) || vadap(S) || vadap(SS)){
            // SPLESH RUN FUNC
            vars newVar;
            newVar.name = defargvarname;
            newVar.valib = lixtVALUE();
            
            auto varh = state.getVariable(vname);
            if(varh && varh->valib.type==runV){ 
                varlib.valib.rargs.push_back(newVar); state.addVariable(varlib);
                
                std::string virtualcode = varh->valib.value;
                std::vector<TOKENS> tokensrunvs = tokenizer(virtualcode);
                urwerer cp_state = state;
                kairrer_tpr ktpr(tokensrunvs, cp_state);

                varlib.valib.type = runV;
                varlib.valib.rargs = ktpr.prun(varlib.valib.rargs);
                for (auto retvar : varlib.valib.rargs) {
                    if (retvar.name == defretvarname) {
                        varlib.valib.type = retvar.valib.type;
                        varlib.valib.value = retvar.valib.value;
                    }
                }
            
            };
            


        }
        /* . */       else if(vadap(TAK)){
            cret(); skipTAB();
            // META DATA VAR
            std::string contextmenuname = lixtSTRING();
            
            if(!contextmenuname.empty()){
                auto varh = state.getVariable(vname);

                if(contextmenuname == "sidant"){
                    if (varh){
                        varlib.valib.value = vname; 
                        varlib.valib.type  = VSID::stringV;
                    
                    }
                    else{
                        varlib.valib.value = get_sname_from_vname(VSID::nullV); 
                        varlib.valib.type  = VSID::stringV;
                    }

                }
                else if(contextmenuname == "ret"){
                    if (varh){
                        varlib.valib.value = get_sname_from_vname(varh->valib.type); 
                        varlib.valib.type  = VSID::stringV;
                    
                    }
                    else{
                        varlib.valib.value = get_sname_from_vname(VSID::nullV); 
                        varlib.valib.type  = VSID::stringV;
                    }
                    
                }
                else if(contextmenuname == "mac"){
                    if (varh){
                        varlib.valib.value = std::to_string( varh->valib.value.size()); 
                        VSID vtype = varh->valib.type;
                        if( vtype  == VSID::hexV){
                            varlib.valib.value = std::to_string( atoi(varh->valib.value.c_str()) ); 
                        }
                        else if (vtype  == VSID::intV){
                            varlib.valib.value = std::to_string( atoi(varh->valib.value.c_str()) ); 
                        }
                        else if(vtype == VSID::nullV){
                            varlib.valib.value = NULL_STR;
                        }
                        else if (vtype == VSID::runV){
                            varlib.valib.value = std::to_string( varh->valib.rargs.size() );
                        }
                        else if(vtype == VSID::stringV){
                            varlib.valib.value = std::to_string( varh->valib.value.size() );
                        }
                        else{
                            varlib.valib.value = NULL_STR;
                        }
                        varlib.valib.type  = VSID::intV;
                    
                    }
                    
                    else{
                        varlib.valib.value = get_sname_from_vname(VSID::nullV); 
                        varlib.valib.type  = VSID::stringV;
                    }
                    
                }
                else{
                    log.fkr("CONTEXTMENU","vah kam sidant -> "+contextmenuname+"; ut var: "+vname+line_pin());
                }

            }
            else{
                log.fkr("CONTEXTMENU","fo vanih et var -> "+vname+"; ten wer: ."+contextmenuname+line_pin());
            }

        }
        else{
            auto varh = state.getVariable(vname);
       
            if(varh){
                varlib.valib=varh->valib;
                varlib.valib=varh->valib;
            }
            else {
                varlib.valib.type=nullV;
            };
        }            
        return varlib;
    }
    std::string lixtPathLiber(){
        cret();
        value vall= lixtVALUE();
        if (vall.type==stringV)return vall.value;
        else log.fkr("LIXTPATHLIB","gat type et lixt liber, ~ <- ut string "+line_pin());
        return NULL_STR;
    }
    void addLiber(std::vector<TOKENS> newtokens){
        std::reverse(newtokens.begin(), newtokens.end());
        for (TOKENS newtoken:newtokens) tokens.insert(tokens.begin() + vap, newtoken);
    }
    
    
    public:
    kairrer_tpr(const std::vector<TOKENS>& tokens, urwerer& state) : tokens(tokens), state(state) {}

    std::vector<vars> prun(std::vector<vars> eninv) { // PARSE AND RUN
        for(vars ev:eninv) state.addVariable(ev);
        std::vector<vars> retvars;

        vars retvar;
        retvar.name=defretvarname;
        retvar.valib.type=VSID::intV;
        retvar.valib.value="0";
        state.addVariable(retvar);
        retvars.push_back(retvar);

        while (!vaZexc()) {
            try {
                //var     
                     if (vadap(TSID::STRING)) {varWer();}
                // else if (esValue()){}
                
                //import
                else if (vadap(TSID::SY))     {addLiber(state.lixtLiber(lixtPathLiber()));}
                
                //comes
                else if (dapin(TSID::DREB))   {skipTAB(); if(dapin(CE)){skipTAB();if(vadap(STRING)){state.removeVariable(varWer().name); } skipLINE(); }else skipLINE(); }
                else if (dapin(TSID::DYR) && lift().type==TSID::DYR) skipLINE();
                //excutes
                else if (dapin(TSID::DIG) && lift().type==TSID::DIG) std::cout<<state.executeCommand(lixtShell());
                else if (dapin(TSID::ZIG) && lift().type==TSID::ZIG) asmcode.executeASM(lixtAsm());
                else if (dapin(TSID::TAK) && lift().type==TSID::TAK) hexcode.executeHex(lixtHex());
                //skipis
                else cret();

            } catch (const std::exception& e) {
                log.fkr("PARS","token " + std::to_string(vap) + ": " + e.what() + "\n "+line_pin());
            }
        }
        
        retvars = state.getVariables(); 
        return retvars;
    }

};
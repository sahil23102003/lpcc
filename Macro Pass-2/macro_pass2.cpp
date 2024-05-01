#include <iostream>
#include <fstream>
#include <string>
#include<unordered_map>
#include<vector>
#include<sstream>

using namespace std;


vector<string> MDT;

class MNT_class{
    public:
    string macro_name;
    int arg_num;
    int start_index;

    MNT_class(string macro_name,int arg_num, int start_index){
        this->macro_name = macro_name;
        this->arg_num = arg_num;
        this->start_index = start_index;
    }
};
vector<MNT_class> MNT;

unordered_map<string,vector<string>> ALA;

int find_MNT(string macro_name){
    for(int i=0;i<MNT.size();i++){
        if(MNT[i].macro_name==macro_name){
            return i;
        }
    }
    return -1;
}

void str_tokenizer(string s, char del)
{
    stringstream ss(s);
    string word;
    while (!ss.eof()) {
        getline(ss, word, del);
        cout << word << endl;
    }
}

// void expand_macro(int start_index,unordered_map<string,string> params){
//     for(int i=start_index;MDT[i]!="MEND";i++){
//         string word;
//         for(int j=0;j<=MDT[i].length();j++){
//             if(MDT[i][j]==' ' || MDT[i][j]==',' || MDT[i][j]=='\0'){
//                 if(word[0]=='&'){
//                     // cout<<"in ininin:"<<word<<endl;
//                     string mdt_line = MDT[i];
//                     mdt_line.replace(j-word.size(),j,params[word]);
//                     cout<<mdt_line.substr(j-word.size(),word.size());
//                 }else{
//                     cout<<word<<" ";
//                 }

//                 word.erase();
//             }else{
//                 word+=MDT[i][j];
//             }
            
//         }
//         cout<<endl;
//     }
// }

void expand_macro(int start_index,unordered_map<string,string> params){
    
    bool macro_call = false;
    bool macro_def = false;

    for(int i=start_index;MDT[i]!="MEND";i++){
        string word;
        skip_line:
        for(int j=0;j<=MDT[i].length();j++){
            if(MDT[i][j]==' ' || MDT[i][j]==',' || MDT[i][j]=='\0'){
                if(word[0]=='&'){
                    // cout<<"in ininin:"<<word<<endl;
                    string mdt_line = MDT[i];
                    mdt_line.replace(j-word.size(),j,params[word]);
                    cout<<mdt_line.substr(j-word.size(),word.size());
                }
                else if(find_MNT(word)!=-1 && macro_def==false){
                    stringstream ss(MDT[i].substr(j+1));
                    string macro_param;
                    unordered_map<string,string> parameters;
                    int args=0;
                    while (!ss.eof()) {
                        getline(ss, macro_param, ',');
                        if(macro_param[0]=='&'){
                            string param_key = macro_param;
                            macro_param.erase();
                            macro_param = params[param_key];
                            parameters[ALA[word][args]] = macro_param;
                            args++;
                            macro_call = true;
                        }else{
                            macro_call=true;
                            parameters[ALA[word][args]] = macro_param;
                            args++;
                        }
                    }
                    if(macro_call==true && args==MNT[find_MNT(word)].arg_num){
                        cout<<"expand "<<MNT[find_MNT(word)].macro_name<<":"<<endl;
                        expand_macro(MNT[find_MNT(word)].start_index,parameters);
                        // getline(inputFile,line);
                        i=i+1;
                        word.erase();
                        // word = line;
                        goto skip_line;
                    }
                }
                else{
                    cout<<word<<" ";
                }

                if(word=="MACRO"){
                    macro_def = true;
                }
                if(word=="MEND"){
                    macro_def = false;
                }

                word.erase();
            }else{
                word+=MDT[i][j];
            }
            
        }
        cout<<endl;
        
    }
}


void Pass1(ifstream &inputFile){
    string line;
    bool macro = false;
    int macro_line_count=0;
    int arg=0;
    vector<string> parameters;
    string macro_name;
    while (getline(inputFile, line)) {
        string word;
        // cout<<line<<endl;
        for(int i=0;i<=line.length();i++){
            
            if(line[i]==' ' || line[i]==',' || line[i]=='\0'){
                if(word=="MACRO"){
                    macro=true;
                    word.erase();
                    continue;
                }
                if(macro==true && macro_line_count==0 && word[0]=='&'){
                    arg++;
                    parameters.emplace_back(word);
                    if(line[i]=='\0'){
                        macro_line_count++;
                        // cout<<"in"<<endl;
                        MNT.push_back(MNT_class(macro_name,arg,MDT.size()));
                        ALA[macro_name] = parameters;
                        parameters.clear();
                    }
                }
                if(macro==true && macro_line_count==0 && word[0]!='&' && word!=""){
                    macro_name = word;
                    // cout<<"Macro name:"<<macro_name<<endl;
                }

                if(macro==true && macro_line_count>=1){
                    do{
                        getline(inputFile,line);
                        MDT.push_back(line);
                        // cout<<"Macro Next Line: "<<line<<endl;
                        macro_line_count++;
                    }while(line!="MEND");
                    macro=false;
                    arg=0;
                    macro_line_count=0;
                    macro_name.erase(); 
                    word="MEND";
                }
                // cout<<word<<" ";
                word.erase();
            }else{
                word += line[i];
            }
        }
        // cout<<endl;
    }

}

void Pass2(ifstream &inputFile){
    string line;
    cout<<endl;
    bool macro_call = false;
    bool macro_def = false;
    while(getline(inputFile,line)){
        string word;
        skip_line:
        for(int i=0;i<=line.length();i++){
            if(line[i]==' ' || line[i]==',' || line[i]=='\0'){

                if(word=="MACRO"){
                    macro_def = true;
                }
                if(word=="MEND"){
                    macro_def = false;
                }

                if(find_MNT(word)!=-1 && macro_def==false){
                    stringstream ss(line.substr(i+1));
                    string macro_param;
                    unordered_map<string,string> parameters;
                    int args=0;
                    while (!ss.eof()) {
                        getline(ss, macro_param, ',');
                        if(macro_param[0]=='&'){
                            macro_call=false;
                            break;
                        }else{
                            parameters[ALA[word][args]] = macro_param;
                            args++;
                            macro_call = true;
                        }
                    }
                    if(macro_call==true && args==MNT[find_MNT(word)].arg_num){
                        cout<<"expand "<<MNT[find_MNT(word)].macro_name<<":"<<endl;
                        expand_macro(MNT[find_MNT(word)].start_index,parameters);
                        getline(inputFile,line);
                        word.erase();
                        // word = line;
                        goto skip_line;
                    }
                }
                cout<<word<<" ";

                word.erase();
            }else{
                word+=line[i];
            }
        }cout<<endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }


    ifstream inputFile(argv[1]);

    
    if (!inputFile) {
        cout << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    Pass1(inputFile);

    inputFile.clear();
    inputFile.seekg(0);

    Pass2(inputFile);

    inputFile.close();

    cout<<endl<<"MNT:"<<endl;
    for(auto itr=MNT.begin();itr!=MNT.end();itr++){
        cout<<"Name:"<<itr->macro_name<<" Arguments = "<<itr->arg_num<<" Start = "<<itr->start_index<<endl;
    }

    cout<<endl<<"MDT:"<<endl;
    for(auto itr=MDT.begin();itr!=MDT.end();itr++){
        cout<<*itr<<endl;
    }

    cout<<endl<<"ALA:"<<endl;
    for(auto itr:ALA){
        cout<<itr.first<<": ";
        for(auto i: itr.second){
            cout<<i<<",";
        }
        cout<<endl;
    }

    return 0;
}

#include <iostream>
#include <fstream>
#include <string>
// #include<unordered_map>
#include<vector>

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

   
    string line;
    bool macro = false;
    int macro_line_count=0;
    int arg=0;
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
                    if(line[i]=='\0'){
                        macro_line_count++;
                        // cout<<"in"<<endl;
                        MNT.push_back(MNT_class(macro_name,arg,MDT.size()));
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

    cout<<endl<<"MNT:"<<endl;
    for(auto itr=MNT.begin();itr!=MNT.end();itr++){
        cout<<"Name:"<<itr->macro_name<<" Arguments = "<<itr->arg_num<<" Start = "<<itr->start_index<<endl;
    }

    cout<<endl<<"MDT:"<<endl;
    for(auto itr=MDT.begin();itr!=MDT.end();itr++){
        cout<<*itr<<endl;
    }

    inputFile.close();

    return 0;
}

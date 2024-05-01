#include <iostream>
#include <fstream>
#include <string>
#include<unordered_map>
#include<vector>

using namespace std;

unordered_map<string,unordered_map<string,string>> EMOT = {{"STOP",{{"class","1"},{"opcode","00"}}},
							     {"ADD",{{"class","1"},{"opcode","01"}}},
							     {"SUB",{{"class","1"},{"opcode","02"}}},
							     {"MULT",{{"class","1"},{"opcode","03"}}},
							     {"MOVER",{{"class","1"},{"opcode","04"}}},
							     {"MOVEM",{{"class","1"},{"opcode","05"}}},
							     {"CMP",{{"class","1"},{"opcode","06"}}},
							     {"BC",{{"class","1"},{"opcode","07"}}},
							     {"DIV",{{"class","1"},{"opcode","08"}}},
							     {"READ",{{"class","1"},{"opcode","09"}}},
							     {"PRINT",{{"class","1"},{"opcode","10"}}},
							     {"START",{{"class","3"},{"opcode","01"}}},
							     {"END",{{"class","3"},{"opcode","02"}}},
							     {"ORIGIIN",{{"class","3"},{"opcode","03"}}},
							     {"EQU",{{"class","3"},{"opcode","04"}}},
							     {"LTORG",{{"class","3"},{"opcode","05"}}},
							     {"DS",{{"class","2"},{"opcode","01"}}},
							     {"DC",{{"class","2"},{"opcode","02"}}},
							     {"AREG",{{"class","4"},{"opcode","01"}}},
							     {"BREG",{{"class","4"},{"opcode","02"}}},
							     {"CREG",{{"class","4"},{"opcode","03"}}},
							     {"EQ",{{"class","5"},{"opcode","01"}}},
							     {"LT",{{"class","5"},{"opcode","02"}}},
							     {"GT",{{"class","5"},{"opcode","03"}}},
							     {"NE",{{"class","5"},{"opcode","04"}}},
							     {"LE",{{"class","5"},{"opcode","05"}}},
							     {"GT",{{"class","5"},{"opcode","06"}}},
							     {"ANY",{{"class","5"},{"opcode","07"}}}	     
};

unordered_map<string,string> statement_type = {{"1","IS"},
                                               {"2","DL"},
                                               {"3","AD"},
                                               {"4","RG"},
                                               {"5","CC"},
};

// unordered_map<string,int> ST;

class ST_class{
    public:
    string symbol;
    int address;

    ST_class(string symbol,int address){
        this->symbol = symbol;
        this->address = address;
    }
};

int return_symbol_index(vector<ST_class> ST,string symbol){
    for (int i=0;i<ST.size(); i++){
        if(ST[i].symbol==symbol){
            return i+1;
        }
    }
    return -1;
}

vector<ST_class> ST;

class LT_class{
    public:
    string literal;
    int address;

    LT_class(string literal,int address){
        this->literal = literal;
        this->address = address;
    }

};

vector<int> PT;

vector<LT_class> LT;

int return_lit_add(vector<LT_class> LT,string literal){
    for (auto itr = LT.begin();itr != LT.end(); itr++){
        if(itr->literal==literal){
            return itr->address;
        }
    }
    return 0;
}

int return_lit_index(vector<LT_class> LT,string literal){
    for (int i=0;i<LT.size(); i++){
        if(LT[i].literal==literal){
            return i+1;
        }
    }
    return -1;
}

void Pass1(ifstream &inputFile){
    string line;
    int LC=0;
    while (getline(inputFile, line)) {
        string word;
        int word_count=0;
        for(int i=0;i<=line.length();i++){
            if(line[i]==' ' || line[i]==',' || line[i]=='=' || line[i]=='\0'){
                word_count++;

                //Print the entire input Assembly Code:
                // cout<<LC<<" "<<word<<endl;
                
                if(word=="START"){
                    LC = stoi(line.substr(i+1))-1;
                    
                }
                if(line[i]=='='){
                    string literal = line.substr(i+1);
                    if(LT.size()==0){
                        PT.emplace_back(0);
                    }
                    if(return_lit_add(LT,literal)!=-1){
                        LT.emplace_back(LT_class(literal,-1));
                    }
                }
                if(word=="LTORG" || word=="END"){
                    for (auto itr = LT.begin();itr != LT.end(); itr++){
                        if(itr->address==-1){
                            itr->address = LC;
                            if(itr!=LT.end()-1){
                                LC++;
                            }
                        }
                    }
                    // Add starting index of the next literal pool at the LTORG or END. 
                    // Hence, we will have to delete the last entry of PT after we finish parsing.
                    PT.emplace_back(LT.size());
                }

                if(word=="DS"){
                    int leap = stoi(line.substr(i+1));
                    LC += leap-1;
                }

                if(EMOT.find(word)==EMOT.end() && word_count==1){
                    // Is a Symbol and add to Symbol Table.
                    // ST[word] = LC;
                    ST.emplace_back(ST_class(word,LC));
                }
                // if(EMOT.find(word)==EMOT.end() && word_count!=1){
                //     // Is a Literal and add to Literal Table.
                //     LT[word] = -1;
                // }
                word.erase();

            }else{
                word += line[i];
            }
            // cout << line[i]; 
        }
        // cout<<endl;
        LC++;
    }
    // Deleting last entry of PT. Explanation is above in 'LTORG' condition block.
    PT.erase(PT.end());
}

// Basically just Intermediate Code(IC) generation
void Pass2(ifstream &inputFile){
    cout<<endl<<"Intermediate Code:"<<endl;
    string line;
    int LC = 0;
    while(getline(inputFile,line)){
        string word;
        cout<<LC<<"|\t";
        for(int i=0;i<=line.length();i++){
            if(line[i]==' ' || line[i]==',' || line[i]=='=' || line[i]=='\0'){
                // cout<<word<<" ";
                if(EMOT.find(word)!=EMOT.end()){
                    cout<<"("<<statement_type[EMOT[word]["class"]]<<","<<EMOT[word]["opcode"]<<") ";
                }
                if(word=="START"){
                    LC = stoi(line.substr(i+1));
                }
                if(return_symbol_index(ST,word)!=-1){
                    cout<<"(ST,"<<return_symbol_index(ST,word)<<") ";
                }
                if(return_lit_index(LT,word)!=-1){
                    cout<<"(LT,"<<return_lit_index(LT,word)<<") ";
                }
                if(word=="DS"){
                    int leap = stoi(line.substr(i+1));
                    LC += leap-1;
                }

                word.erase();
            }else{
                word+=line[i];
            }
        }
        cout<<endl;
        LC++;
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

    cout<<endl<<"Symbol Table:"<<endl;
    for (auto itr = ST.begin();itr != ST.end(); itr++){
        cout << itr->symbol << " " <<  itr->address << endl;
    }

    cout<<endl<<"Literal Table:"<<endl;
    for (auto itr = LT.begin();itr != LT.end(); itr++){
        cout << itr->literal << " " <<  itr->address << endl;
    }

    cout<<endl<<"Pool Table:"<<endl;
    for (auto itr = PT.begin();itr != PT.end(); itr++){
        cout << *itr << endl;
    }

    return 0;
}
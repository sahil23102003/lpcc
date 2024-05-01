#include <bits/stdc++.h>
using namespace std;

unordered_map<string, unordered_map<string, string>> EMOT = {{"STOP", {{"class", "1"}, {"opcode", "00"}}},
                                                             {"ADD", {{"class", "1"}, {"opcode", "01"}}},
                                                             {"SUB", {{"class", "1"}, {"opcode", "02"}}},
                                                             {"MULT", {{"class", "1"}, {"opcode", "03"}}},
                                                             {"MOVER", {{"class", "1"}, {"opcode", "04"}}},
                                                             {"MOVEM", {{"class", "1"}, {"opcode", "05"}}},
                                                             {"CMP", {{"class", "1"}, {"opcode", "06"}}},
                                                             {"BC", {{"class", "1"}, {"opcode", "07"}}},
                                                             {"DIV", {{"class", "1"}, {"opcode", "08"}}},
                                                             {"READ", {{"class", "1"}, {"opcode", "09"}}},
                                                             {"PRINT", {{"class", "1"}, {"opcode", "10"}}},
                                                             {"START", {{"class", "3"}, {"opcode", "01"}}},
                                                             {"END", {{"class", "3"}, {"opcode", "02"}}},
                                                             {"ORIGIIN", {{"class", "3"}, {"opcode", "03"}}},
                                                             {"EQU", {{"class", "3"}, {"opcode", "04"}}},
                                                             {"LTORG", {{"class", "3"}, {"opcode", "05"}}},
                                                             {"DS", {{"class", "2"}, {"opcode", "01"}}},
                                                             {"DC", {{"class", "2"}, {"opcode", "02"}}},
                                                             {"AREG", {{"class", "4"}, {"opcode", "01"}}},
                                                             {"BREG", {{"class", "4"}, {"opcode", "02"}}},
                                                             {"CREG", {{"class", "4"}, {"opcode", "03"}}},
                                                             {"EQ", {{"class", "5"}, {"opcode", "01"}}},
                                                             {"LT", {{"class", "5"}, {"opcode", "02"}}},
                                                             {"GT", {{"class", "5"}, {"opcode", "03"}}},
                                                             {"NE", {{"class", "5"}, {"opcode", "04"}}},
                                                             {"LE", {{"class", "5"}, {"opcode", "05"}}},
                                                             {"GT", {{"class", "5"}, {"opcode", "06"}}},
                                                             {"ANY", {{"class", "5"}, {"opcode", "07"}}}};

unordered_map<string, string> statement_type = {
    {"1", "IS"},
    {"2", "DL"},
    {"3", "AD"},
    {"4", "RG"},
    {"5", "CC"},
};

class symbol_table
{
public:
    string literal;
    int add;
    symbol_table(string literal, int add)
    {
        this->literal = literal;
        this->add = add;
    }
};

class literal_table
{
public:
    string literal;
    int add;
    literal_table(string literal, int add)
    {
        this->literal = literal;
        this->add = add;
    }
};
// = {{"LOOP", 182}, {"COMP", 187}, {"BACK", 186}, {"M", 190}, {"N", 191}} = {{"200", 192}, {"500", 193}}
vector<symbol_table> ST;
vector<literal_table> LT;

int return_symbol_index(vector<symbol_table> ST, string symbol)
{
    for (int i = 0; i < ST.size(); i++)
    {
        if (ST[i].literal == symbol)
        {
            return i + 1;
        }
    }
    return -1;
}

int return_lit_index(vector<literal_table> LT, string literal)
{
    for (int i = 0; i < LT.size(); i++)
    {
        if (LT[i].literal == literal)
        {
            return i + 1;
        }
    }
    return -1;
}

bool check_lit(vector<literal_table> LT, string lit)
{
    for (auto it = LT.begin(); it != LT.end(); it++)
    {
        if (it->literal == lit)
        {
            return true;
        }
    }
    return false;
}

void pass1(ifstream &inputfile)
{
    string line;
    int LC = 0;
    while (getline(inputfile, line))
    {
        string word;
        int wc = 0;
        // read each line proccess each word check it
        for (int i = 0; i <= line.length(); i++)
        {
            if (line[i] == ' ' || line[i] == ',' || line[i] == '=' || line[i] == '\0')
            {
                wc++;
                // check if word is START statement then update LC
                if (word == "START")
                {
                    LC = stoi(line.substr(i + 1)) - 1;
                }
                // check for literals
                if (line[i] == '=')
                {
                    // iska mtlb literal hai aur abhi isko literal table mai dalna hai
                    //  pr koi add ke bina i.e -1
                    string lit = line.substr(i + 1);
                    if (check_lit(LT, lit) == false)
                    {
                        // literal phele se literal table mai nhi hai to daldo
                        LT.emplace_back(literal_table(lit, -1));
                    }
                }
                if (word == "LTORG" || word == "END")
                {
                    // literal table mai entry kro un literal ki jinka add -1 hai
                    for (auto it = LT.begin(); it != LT.end(); it++)
                    {
                        if (it->add == -1)
                        {
                            it->add = LC;
                            if (it != LT.end() - 1)
                            {
                                LC++;
                            }
                        }
                    }
                }
                if (word == "DS")
                {
                    int leap = stoi(line.substr(i + 1));
                    LC += leap - 1;
                }

                // symbol bahrna hai ab wordcount 1 hai aur EMOT mai nhi hai
                // mtlb symbol hai
                if (EMOT.find(word) == EMOT.end() and wc == 1)
                {
                    ST.emplace_back(symbol_table(word, LC));
                }

                word.erase();
            }
            else
            {
                word += line[i];
            }
        }
        LC++;
    }
}

int main(int argc, char const *argv[])
{
    ifstream inputfile(argv[1]);
    pass1(inputfile);


    inputfile.close();

    cout << "literal table" << endl;
    for (auto it = LT.begin(); it != LT.end(); it++)
    {
        cout << it->literal << " " << it->add << " " << endl;
    }

    cout << "symbol table" << endl;
    for (auto it = ST.begin(); it != ST.end(); it++)
    {
        cout << it->literal << " " << it->add << " " << endl;
    }

    ifstream inputfile1(argv[1]);
     cout<<endl<<"Intermediate Code:"<<endl;
    string line;
    int LC = 0;
    while(getline(inputfile1,line)){
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
    inputfile1.close();
    return 0;
}

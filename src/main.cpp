#include<iostream>
#include<string>
#include<fstream>   
#include<map>
using namespace std;

int main () {
    
    map<string, string> variables;

    cout << "NIKLANG Interpreter started!" << endl;

    string command = "bol";     // command is a string variable that stores 'bol'.

    // cout << "Command: " << command << endl; 


    // if (command == "bol") {
    //     cout << "NikLang Output Command Detected!" << endl; 
    // } else {
    //     cout << "Unknown NikLang Command" << endl;
    // }

    ifstream MyFile("./examples/hello.nik");

    if (MyFile) {
        // cout << "File Opend Successfully!" << endl;

        string line;

        while(getline(MyFile, line)) {
            
            if(line.find("bol") == 0) {

                int firstQuote = line.find('"');
                int secondQuote = line.find('"',firstQuote + 1);

                string final = line.substr(firstQuote+1, (secondQuote-firstQuote-1));
                
                if (line.find("+") != string::npos) {
                    int plusPos = line.find("+");
                    int variableStart = line.find_first_not_of(" ", plusPos+1);
                    int variableEnd = line.find_last_not_of(" ");  
                    string variableName = line.substr(variableStart, variableEnd-variableStart + 1);

                    if (variables.find(variableName) != variables.end()) {
                        string variableValue = variables[variableName];

                        cout << final << variableValue << endl;
                    } else {
                        cout << "NIKLANG ERROR : variable '" << variableName << "' is not defined!" << endl;
                    }

                } else {
                    cout << final << endl;
                }
            }

            if(line.find("maanlo ") == 0) {

                int varNameStart = line.find(" ");
                int varNameEnd = line.find(" =", varNameStart+1);
                string varName = line.substr(varNameStart+1, (varNameEnd-varNameStart-1));
                // cout << varName << endl;

                int varValueStart = line.find("= ");
                string varValue = line.substr(varValueStart+2);
                // cout << varValue << endl;

                if (varValue.find("puch(") != string::npos) {

                    int firstQuote = varValue.find('"');
                    int secondQuote = varValue.find('"', firstQuote+1);

                    string final = varValue.substr(firstQuote+1, secondQuote-firstQuote-1);

                    cout << final;

                    string userInput;
                    getline(cin, userInput);
                    
                    varValue = userInput;
                }

                if (variables.find(varValue) != variables.end()) {
                    varValue = variables[varValue];
                }

                if (varValue.find("-") != string::npos) {
                    int leftNumber, rightNumber;

                    int minus = varValue.find("-");
                    string leftSide = varValue.substr(0, minus-1);
                    string rightSide = varValue.substr(minus+2);

                    if (variables.find(leftSide) != variables.end() && variables.find(rightSide) != variables.end()) {
                        leftNumber = stoi(variables[leftSide]);
                        rightNumber = stoi(variables[rightSide]);
                        // cout << leftNumber << "-" << rightNumber << "= " << leftNumber-rightNumber << endl;

                        int result = leftNumber - rightNumber;
                        varValue = to_string(result);

                    }

                    if(variables.find(leftSide) == variables.end()) {
                        cout << "NIKLANG ERROR : variable '" << leftSide << "' is not defined!" << endl;

                    }

                    if(variables.find(rightSide) == variables.end()) {
                        cout << "NIKLANG ERROR : variable '" << rightSide << "' is not defined!" << endl;

                    }

                }

                variables[varName] = varValue;
                // cout << "Stored value: " << variables[varName] << endl;

                // cout << varName << " = " << variables[varName] << endl;
            }
        }
    } else {
        cout << "Could not open the file!" << endl;
    }

    return 0;
}
    #include<iostream>
    #include<string>
    #include<fstream>   
    #include<map>
    #include<stdexcept>
    using namespace std;

    double calculate(double leftNumber, double rightNumber, char operation) {

        switch(operation) {
            case '+' :
                return leftNumber + rightNumber;

            case '-' :
                return leftNumber - rightNumber;

            case '*' :
                return leftNumber * rightNumber;
            
            case '/' :
                return leftNumber / rightNumber;

            default :
                return 0;
        }

    }

    bool variableExists(const map<string, string>& variables, const string& variableName) {
        return variables.find(variableName) != variables.end();
    }

    size_t findOperator(const string& value) {

        size_t position = value.find_first_of("+-*/");

        if (position == string::npos) {
            return string::npos;

        } else if (position == 0 && value[position] == '-') {
            return string::npos;

        } else {
            return position;
        
        }
    }

    string trim(const string& value) {
        size_t start = value.find_first_not_of(" ");
        size_t end = value.find_last_not_of(" ");

        if (start == string::npos) {
            return "";
        }
        
        return value.substr(start, end-start + 1);
    }

    void splitExpression(const string& expression) {

        size_t position = findOperator(expression);

        if (position != string::npos) {
            string leftSide = trim(expression.substr(0, position));
            string rightSide = trim(expression.substr(position+1));

            cout << "Left: " << leftSide << endl;
            cout << "Right: " << rightSide << endl;
        } else {
            return;
        }
    }

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

                        bool variableFound = variableExists(variables, variableName);
                        if (variableFound) {
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

                    bool calculationSuccessful = true;
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

                    bool varValueExists = variableExists(variables, varValue);
                    if (varValueExists) {
                        varValue = variables[varValue];
                    }

                    size_t minus = findOperator(varValue);
                    if (minus != string::npos && minus > 0) {
                        double leftNumber, rightNumber;
                        
                        string leftSide = varValue.substr(0, minus-1);
                        string rightSide = varValue.substr(minus+2);

                        bool leftExists = variableExists(variables, leftSide);
                        bool rightExists = variableExists(variables, rightSide);
                        if (leftExists && rightExists) {
                            try {
                                leftNumber = stod(variables[leftSide]);
                                rightNumber = stod(variables[rightSide]);
                                // cout << leftNumber << "-" << rightNumber << "= " << leftNumber-rightNumber << endl;

                                double result = calculate(leftNumber, rightNumber, '-');
                                varValue = to_string(result);

                                calculationSuccessful = true;

                            } catch (const invalid_argument& e) {

                                cout << "NIKLANG ERROR: cannot perform artithmetic operation on non-numeric value." << endl;
                                calculationSuccessful = false;

                            } catch (const out_of_range& o) {
                                cout << "NIKLANG ERROR: this number is too large to perform artithmetic operation." << endl;
                                calculationSuccessful = false;

                            }
                        }

                        if(!leftExists) {
                            cout << "NIKLANG ERROR : variable '" << leftSide << "' is not defined!" << endl;
                            calculationSuccessful = false;
                        }

                        if(!rightExists) {
                            cout << "NIKLANG ERROR : variable '" << rightSide << "' is not defined!" << endl;
                            calculationSuccessful = false;
                        }

                    } else if (varValue.find("+") != string::npos) {
                        double leftNumber, rightNumber;

                        size_t plus = varValue.find("+");
                        string leftSide = varValue.substr(0, plus-1);
                        string rightSide = varValue.substr(plus+2);

                        bool leftExists = variableExists(variables, leftSide);
                        bool rightExists = variableExists(variables, rightSide);

                        if (leftExists && rightExists) {
                            try {
                                leftNumber = stod(variables[leftSide]);
                                rightNumber = stod(variables[rightSide]);
                                // cout << leftNumber << "-" << rightNumber << "= " << leftNumber-rightNumber << endl;

                                double result = calculate(leftNumber, rightNumber, '+');

                                varValue = to_string(result);

                                calculationSuccessful = true;

                            } catch (const invalid_argument& e) {

                                cout << "NIKLANG ERROR: cannot perform artithmetic operation on non-numeric value." << endl;
                                calculationSuccessful = false;

                            } catch (const out_of_range& o) {
                                cout << "NIKLANG ERROR: this number is too large to perform artithmetic operation." << endl;
                                calculationSuccessful = false;

                            }
                        }

                        if(!leftExists) {
                            cout << "NIKLANG ERROR : variable '" << leftSide << "' is not defined!" << endl;
                            calculationSuccessful = false;
                        }

                        if(!rightExists) {
                            cout << "NIKLANG ERROR : variable '" << rightSide << "' is not defined!" << endl;
                            calculationSuccessful = false;
                        }

                    } else if (varValue.find("*") != string::npos) {
                        double leftNumber, rightNumber;

                        size_t multiply = varValue.find("*");
                        string leftSide = varValue.substr(0, multiply-1);
                        string rightSide = varValue.substr(multiply+2);

                        bool leftExists = variableExists(variables, leftSide);
                        bool rightExists = variableExists(variables, rightSide);
                        if (leftExists && rightExists) {
                            try {
                                leftNumber = stod(variables[leftSide]);
                                rightNumber = stod(variables[rightSide]);
                                // cout << leftNumber << "-" << rightNumber << "= " << leftNumber-rightNumber << endl;

                                double result = calculate(leftNumber, rightNumber, '*');

                                varValue = to_string(result);

                                calculationSuccessful = true;

                            } catch (const invalid_argument& e) {

                                cout << "NIKLANG ERROR: cannot perform artithmetic operation on non-numeric value." << endl;
                                calculationSuccessful = false;

                            } catch (const out_of_range& o) {
                                cout << "NIKLANG ERROR: this number is too large to perform artithmetic operation." << endl;
                                calculationSuccessful = false;

                            }
                        }

                        if(!leftExists) {
                            cout << "NIKLANG ERROR : variable '" << leftSide << "' is not defined!" << endl;
                            calculationSuccessful = false;
                        }

                        if(!rightExists) {
                            cout << "NIKLANG ERROR : variable '" << rightSide << "' is not defined!" << endl;
                            calculationSuccessful = false;
                        }

                    } else if (varValue.find("/") != string::npos) {
                        double leftNumber, rightNumber;

                        size_t divide = varValue.find("/");
                        string leftSide = varValue.substr(0, divide-1);
                        string rightSide = varValue.substr(divide+2);

                        bool leftExists = variableExists(variables, leftSide);
                        bool rightExists = variableExists(variables, rightSide);

                        if (leftExists && rightExists) {
                            try {
                                leftNumber = stod(variables[leftSide]);
                                rightNumber = stod(variables[rightSide]);
                                // cout << leftNumber << "-" << rightNumber << "= " << leftNumber-rightNumber << endl;


                                if (rightNumber == 0) {
                                    cout << "NIKLANG ERROR: The number is cannot devidable by zero." << endl;
                                    calculationSuccessful = false;
                                } else {

                                    double result = calculate(leftNumber, rightNumber, '/');

                                    varValue = to_string(result);

                                    calculationSuccessful = true;
                                }

                                

                            } catch (const invalid_argument& e) {

                                cout << "NIKLANG ERROR: cannot perform artithmetic operation on non-numeric value." << endl;
                                calculationSuccessful = false;

                            } catch (const out_of_range& o) {
                                cout << "NIKLANG ERROR: this number is too large to perform artithmetic operation." << endl;
                                calculationSuccessful = false;

                            } 
                        }

                        if(!leftExists) {
                            cout << "NIKLANG ERROR : variable '" << leftSide << "' is not defined!" << endl;
                            calculationSuccessful = false;
                        }

                        if(!rightExists) {
                            cout << "NIKLANG ERROR : variable '" << rightSide << "' is not defined!" << endl;
                            calculationSuccessful = false;
                        }

                    }

                    if (calculationSuccessful) {
                        variables[varName] = varValue;
                    }
                    // cout << "Stored value: " << variables[varName] << endl;

                    // cout << varName << " = " << variables[varName] << endl;
                }
            }
        } else {
            cout << "Could not open the file!" << endl;
        }

        return 0;
    }



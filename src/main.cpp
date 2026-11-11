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

    pair<string, string> splitExpression(const string& expression) {

        size_t position = findOperator(expression);

        if (position != string::npos) {
            string leftSide = trim(expression.substr(0, position));
            string rightSide = trim(expression.substr(position+1));

            return {leftSide, rightSide};
        } else {
            return {"", ""};
        }
    }

    bool compareValues(double leftValue, double rightValue, const string& operation) {
        if (operation == ">=") {
            return (leftValue >= rightValue);

        } else if (operation == "<=") {
            return (leftValue <= rightValue);

        } else if (operation == "==") {
            return (leftValue == rightValue);

        } else if (operation == "!=") {
            return (leftValue != rightValue);

        } else if (operation == ">") {
            return (leftValue > rightValue);

        } else if (operation == "<") {
            return (leftValue < rightValue);

        } else {
            return false;
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
                    
                    size_t position = findOperator(varValue);
                        if (position != string::npos) {
                            pair<string, string> operands = splitExpression(varValue);

                            string leftSide = operands.first;
                            string rightSide = operands.second;
                            char operation = varValue[position];

                            bool leftExists = variableExists(variables, leftSide);
                            bool rightExists = variableExists(variables, rightSide);

                            if(!leftExists) {
                                cout << "NIKLANG ERROR: variable '" + leftSide + "' is not defined!" << endl;
                                calculationSuccessful = false;
                            }

                            if(!rightExists) {
                                cout << "NIKLANG ERROR: variable '" + rightSide + "' is not defined!" << endl;
                                calculationSuccessful = false;
                            }

                            if (leftExists && rightExists) {
                                try {
                                    double leftNumber = stod(variables[leftSide]);
                                    double rightNumber = stod(variables[rightSide]);

                                    if (operation == '/' && rightNumber == 0) {

                                        cout << "NIKLANG ERROR: Cannot devide by zero." << endl;
                                        calculationSuccessful = false;
                                    } else {

                                        double result = calculate(leftNumber, rightNumber, operation);

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

                        }
                    if (calculationSuccessful) {
                        variables[varName] = varValue;
                    }
                }

                if (line.find("agar ") == 0) {
                    size_t conditionStart = line.find("agar ") + 5;
                    size_t conditionEnd = line.find("{");

                    string condition = line.substr(conditionStart, conditionEnd-conditionStart);

                    string comparisionOperator;
                    size_t operatorPosition = string::npos;

                    if(condition.find(">=") != string::npos) {
                        comparisionOperator = ">=";
                        operatorPosition = condition.find(">=");

                    } else if (condition.find("<=") != string::npos) {
                        comparisionOperator = "<=";
                        operatorPosition = condition.find("<=");

                    } else if (condition.find("==") != string::npos) {
                        comparisionOperator = "==";
                        operatorPosition = condition.find("==");

                    } else if (condition.find("!=") != string::npos) {
                        comparisionOperator = "!=";
                        operatorPosition = condition.find("!=");

                    } else if (condition.find(">") != string::npos) {
                        comparisionOperator = ">";
                        operatorPosition = condition.find(">");

                    } else if (condition.find("<") != string::npos) {
                        comparisionOperator = "<";
                        operatorPosition = condition.find("<");

                    } else {
                        cout << "NIKLANG ERROR: Invalid Comparision Operator!" << endl;

                    }

                    string leftSide = trim(condition.substr(0, operatorPosition));
                    string rightSide = trim(condition.substr(operatorPosition + comparisionOperator.length(), condition.length() - (operatorPosition + comparisionOperator.length())));

                    // cout << "Left: " << leftSide << endl;
                    // cout << "Operator: " << comparisionOperator << endl;
                    // cout << "Right: " << rightSide << endl;

                    double leftNumber = 0;
                    double rightNumber = 0;

                    try {
                        if (variableExists(variables, leftSide)) {
                            leftNumber = stod(variables[leftSide]);
                        } else {
                            leftNumber = stod(leftSide);
                        }

                        if (variableExists(variables, rightSide)) {
                            rightNumber = stod(variables[rightSide]);
                        } else {
                            rightNumber = stod(rightSide);
                        }

                    } catch (const invalid_argument& e) {

                        cout << "NIKLANG ERROR: cannot perform artithmetic operation on non-numeric value." << endl;
                        calculationSuccessful = false;

                    } catch (const out_of_range& o) {
                        cout << "NIKLANG ERROR: this number is too large to perform artithmetic operation." << endl;
                        calculationSuccessful = false;

                    }
                }
            }
        } else {
            cout << "Could not open the file!" << endl;
        }

        return 0;
    }



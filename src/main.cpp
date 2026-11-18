    #include<iostream>
    #include<string>
    #include<fstream>   
    #include<map>
    #include<stdexcept>
    #include<vector>
    using namespace std;

    // Simple arithmetic helper used by the interpreter when a variable assignment contains
// an expression such as "x = 10 + 5".
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

// Checks whether a variable has already been created in the current scope.
bool variableExists(const map<string, string>& variables, const string& variableName) {
        return variables.find(variableName) != variables.end();
    }

// Finds the first arithmetic operator in an expression. We skip a leading minus sign
// so negative numbers like "-5 + 2" are treated as a value, not as subtraction.
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

// Removes surrounding spaces from a token so expressions like "  x  +  2 "
// can be processed consistently.
string trim(const string& value) {
        size_t start = value.find_first_not_of(" ");
        size_t end = value.find_last_not_of(" ");

        if (start == string::npos) {
            return "";
        }
        
        return value.substr(start, end-start + 1);
    }

// Splits a binary expression into its left and right operands around the operator.
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

// Compares two numeric values using a relational operator such as >, <, or ==.
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

// Prints a literal string and can also append the value of a variable when used in
// the language's output statement.
void executeBol(const string& line, map<string, string>& variables) {

        int firstQuote = line.find('"');
        int secondQuote = line.find('"',firstQuote + 1);

        string final = line.substr(firstQuote+1, (secondQuote-firstQuote-1));

        if (line.find("+") != string::npos) {
            int plusPos = line.find("+");
            int variableStart = line.find_first_not_of(" ", plusPos+1);
            int variableEnd = line.find_last_not_of(" ");  

            // extracts parts of both side's string
            string leftPart = line.substr(" ", pusPos);
            string rightPart = line.substr();

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

// isNumeric is defined below; this declaration lets executeMaanlo use it.
bool isNumeric(const string& value);

// Handles variable assignment. It supports direct numeric assignment, reading input from
// the user with puch(...), and simple arithmetic expressions like "a = 2 + 3".
void executeMaanlo(const string& line, map<string, string>& variables) {
    if(line.find("maanlo ") == 0) {

            bool calculationSuccessful = true;
            int varNameStart = line.find(" ");
            int varNameEnd = line.find(" =", varNameStart+1);
            string varName = line.substr(varNameStart+1, (varNameEnd-varNameStart-1));
            // cout << varName << endl;

            int varValueStart = line.find("= ");
            string varValue = line.substr(varValueStart+2);
            // cout << "[" << varValue << "]" << endl; // temerory debugging block
            
            // checking if varvalue string is empty or not and contains quotes at end or front
            if (!varValue.empty() && varValue.font() == '"' && varValue.back() == '"') {

                // removes extra quotes from string
                varValue = varValue.substr(1, varValue.length() - 2);
                // cout << varValue << endl;
            }

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

            bool leftExists = variableExists(variables, leftSide) || isNumeric(leftSide);
            bool rightExists = variableExists(variables, rightSide) || isNumeric(rightSide);

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
                        double leftNumber;
                        double rightNumber;

                        // Read a variable's stored value, or convert a numeric literal directly.
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
}

// Confirms that the entire value is a valid number and not just a numeric prefix.
bool isNumeric(const string& value) {
    // stod can parse a numeric prefix, so verify how many characters it consumed.
    try {
        size_t position;
        stod(value, &position);

        return position == value.length();
    } catch (...) {
        return false;
    }

}

// Evaluates a condition like "x >= 10" against the current variable values.
bool evaluateCondition(string condition, map<string, string>& variables) {

    string operation;
    size_t position = string::npos;

    if (condition.find(">=") != string::npos) {
        operation = ">=";
    } else if (condition.find("<=") != string::npos) {
        operation = "<=";
    } else if (condition.find("==") != string::npos) {
        operation = "==";
    } else if (condition.find("!=") != string::npos) {
        operation = "!=";
    } else if (condition.find(">") != string::npos) {
        operation = ">";
    } else if (condition.find("<") != string::npos) {
        operation = "<";
    } else {
        cout << "NIKLANG ERROR: Invalid comparison operator!" << endl;
        return false;
    }

    position = condition.find(operation);

    string leftSide = trim(condition.substr(0, position));
    string rightSide = trim(condition.substr(position + operation.length()));

    try {
        double leftNumber;
        double rightNumber;

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

        return compareValues(leftNumber, rightNumber, operation);

    } catch (...) {
        cout << "NIKLANG ERROR: Invalid condition!" << endl;
        return false;
    }
}

    int main () {
        
        // The interpreter keeps all user-defined variables in memory for the life of the script.
        map<string, string> variables;

        cout << "NIKLANG Interpreter started!" << endl;

        // This variable is kept for historical compatibility with the original code and is not used further.
        string command = "bol";

        // Open the .nik program file from the project root folder.
        ifstream MyFile("./examples/hello.nik");

        if (MyFile) {
            string line;

            // Read the program line by line and interpret each command as it appears.
            while(getline(MyFile, line)) {
                
                // A simple print statement may be written as "bol "something"".
                if (line.find("bol") == 0) {
                    executeBol(line, variables);
                }

                // Variable assignment and basic arithmetic happen here.
                if (line.find("maanlo ") == 0) {
                    executeMaanlo(line, variables);
                }

                // Run the loop while its condition stays true.
                if (line.find("jab tak ") == 0) {

                    size_t conditionStart = line.find("jab tak ") + 8;
                    size_t conditionEnd = line.find("{");

                    // this extracts condition
                    string condition = trim(
                        line.substr(conditionStart, conditionEnd - conditionStart)
                    );

                    // Save the loop body so it can run again.
                    vector<string> loopBody;

                    while (getline(MyFile, line)) {

                        if (trim(line) == "}") {    // read's line until closing breaket
                            break;
                        }

                        loopBody.push_back(line);
                    }

                    // Check the condition before every iteration.
                    // Stop the loop if it runs too many times.
                    int iterationCount = 0;

                    while (evaluateCondition(condition, variables)) {

                        if (iterationCount >= 1000) {
                            cout << "NIKLANG ERROR: Loop limit exceeded!" << endl;
                            break;
                        }

                        iterationCount++;

                        for (string currentLine : loopBody) {

                            currentLine = trim(currentLine);

                            if (currentLine.find("bol ") == 0) {
                                executeBol(currentLine, variables);
                            }
                            else if (currentLine.find("maanlo ") == 0) {
                                executeMaanlo(currentLine, variables);
                            }
                        }
                    }
                }

                // Conditional execution starts with "agar ... {" and optional "warna" branch.
                if (line.find("agar ") == 0) {
                    size_t conditionStart = line.find("agar ") + 5;
                    size_t conditionEnd = line.find("{");

                    string condition = line.substr(conditionStart, conditionEnd-conditionStart);

                    string comparisionOperator;
                    size_t operatorPosition = string::npos;

                    // Check two-character comparison operators before single-character ones.
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

                    double leftNumber = 0;
                    double rightNumber = 0;
                    bool conditionResult = false;
                    bool calculationSuccessful = true;
                    
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

                        if (calculationSuccessful) {
                            conditionResult = compareValues(
                                leftNumber, rightNumber, comparisionOperator
                            );

                        }

                    } catch (const invalid_argument& e) {

                        cout << "NIKLANG ERROR: cannot perform artithmetic operation on non-numeric value." << endl;
                        calculationSuccessful = false;

                    } catch (const out_of_range& o) {
                        cout << "NIKLANG ERROR: this number is too large to perform artithmetic operation." << endl;
                        calculationSuccessful = false;

                    }

                    bool executeAgarBlock = calculationSuccessful && conditionResult;
                    bool executeWarnaBlock = calculationSuccessful && !conditionResult;

                    // Only execute the matching branch when the condition is valid.
                    if (calculationSuccessful) {

                        bool warnaFound = false;

                        while (getline(MyFile, line)) {

                            string currentLine = trim(line);

                            if (currentLine == "warna {" || currentLine == "} warna {") {
                                warnaFound = true;
                                break;
                            }

                            if (trim(line) == "}") {
                                break;
                            }

                            if (executeAgarBlock && trim(line).find("bol") == 0) {
                                executeBol(trim(line), variables);
                            }
                        }

                        if(warnaFound) {
                            while (getline(MyFile, line)) {
                                if (trim(line) == "}") {
                                    break;
                                }

                                if(executeWarnaBlock && trim(line).find("bol") == 0) {
                                    executeBol(trim(line), variables);
                                }
                            }
                        }
                    }
                }
            
        }
        } else {
            cout << "Could not open the file!" << endl;
        }

        return 0;
    }



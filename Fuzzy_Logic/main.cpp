#include <bits/stdc++.h>
using namespace std;
using Pair = pair<string, string>;

enum VarType{
    IN, OUT
};
enum FuzzyType {
    TRI,
    TRAP
};
//////////////////////////////// SPLITE FUNCTION ////////////////////////////////////////////////////////////////////
vector <string> Splite(string line){
    string ans = "";
    vector<string >info;
    for (int i = 0; i < line.size(); ++i) {
        if (line[i]==' '){
            info.push_back(ans);
            ans="";
        }
        else ans+=line[i];
        if(i==line.size()-1)info.push_back(ans);
    }
    return info;
}
///////////////////////////////////////////// SETS CLASS ///////////////////////////////////////////////////////////
class FuzzySet {
public:
    string name;
    FuzzyType type;
    vector<double> values;

    FuzzySet(string name, string Type, vector<double> values)
            : name(name), values(values) {
        if (Type == "TRI") {
            type = TRI;
        } else {
            type = TRAP;
        }
    }
    double getMembershipValue(double val)  {
        if (isInRange(val, values.front(), values.back())) {
            for (int i = 0; i < values.size() - 1; i++) {
                if (isInRange(val, values[i], values[i + 1])) {
                    return calculateIntersection(val, i, i + 1);
                }
            }
        }
        return 0.0;
    }
    double calculateIntersection(double val, int i1, int i2) {
        int y1, y2;
        if (type == TRI) {
            if (i1 == 1)y1 = 1;
            else y1 = 0;

            if (i2 == 1)y2 = 1;
            else y2 = 0;
        }
        else {
            if (i1 == 1 || i1 == 2)y1 = 1;
            else y1 = 0;

            if (i2 == 1 || i2 == 2) y2 = 1;
            else y2 = 0;
        }

        double x1 = values[i1];
        double x2 = values[i2];
        double slope = (y1 - y2) / (x1 - x2); //a
        double yIntersection = y1 - ((x1 - val) * slope); //b
        return yIntersection;
    }
    //valid or not
    bool isInRange(double val, double min, double max) {
        return val >= min && val <= max;
    }
};

////////////////////////////////////////////// MEMPERSHIP CLASS ////////////////////////////////////////////////////
class Membership {
public:
    string name;
    double degreeOfMembership;

    Membership(string name, double degreeOfMembership)
            : name(name), degreeOfMembership(degreeOfMembership) {}
};

////////////////////////////////////////////// VARIABLES CLASS ////////////////////////////////////////////////////
static  vector<Membership> memberships;
static vector <FuzzySet> fuzzySets;
static vector<pair<string ,FuzzySet> >vec;

class FuzzyVariable {
public:
    string name;
    VarType type;
    double min, max;

//    vector<string >info;
//    vector<double>Values;
    double crispValue;
    FuzzyVariable(){
        crispValue=-1.0;
    }
    // Getter for min
    double GetMin() {
        return min;
    }

    // Setter for min
    void SetMin(double newMin) {
        min = newMin;
    }

    // Getter for max
    double GetMax() {
        return max;
    }

    // Setter for max
    void SetMax(double newMax) {
        max = newMax;
    }
    void AddFuzzySet(string line , string nam){
        vector<double>Values;
        vector<string >info = Splite(line);
        string FuzzyName=info[0];
        string Type=info[1];
        if(Type=="TRI"||Type=="TRAP"){
            for (int i = 2; i < info.size(); ++i) {
                double doubleValue;
                if(info[i]!=""){
                    int fans=0;
                    string inf=info[i];
                    for (int j = 0; j < inf.size(); ++j) {
                        int ans = inf[j] - '0';
                        fans+=ans;
                        if(j<inf.size()-1)fans*=10;
                    }
                    doubleValue = (double )fans;
//                    cout<<"doubleValue : "<<doubleValue<<endl;
                    if(isInRange(doubleValue))  Values.push_back(doubleValue);
                    else cout<<"the value out of range"<<endl;
                }
            }
            fuzzySets.push_back(FuzzySet(FuzzyName, Type,Values));
            vec.emplace_back(nam , FuzzySet(FuzzyName, Type,Values));
        }
        else cout<<"Invalid Type"<<endl;
    }

    bool isInRange(double val) {
        return val >= GetMin() && val <= GetMax();
    }
    void SetMembership(double value ,string nam){
        int siz=0;
        vector<FuzzySet> anss;
        for (int i = 0; i < vec.size(); ++i) {
            if(vec[i].first == nam) {
                siz++;
                anss.push_back(vec[i].second);
            }
        }
        crispValue = value;
      //  cout<<fuzzySets.size()<<endl;
        for (int i = 0; i < siz; ++i) {
            if(value == -1.0) {
                memberships.push_back(Membership(anss[i].name, -1));
            }
            memberships.push_back(Membership(anss[i].name, anss[i].getMembershipValue(value)));
        }
    }
    string GetMaxMember() {
        double max = memberships[0].degreeOfMembership;
        string Name = memberships[0].name;

        for (int i = 1; i < memberships.size(); i++) {
            if (memberships[i].degreeOfMembership > max) {
                max = memberships[i].degreeOfMembership;
                Name = memberships[i].name;
            } else if (memberships[i].degreeOfMembership == max) {
                Name += ", " + memberships[i].name;
            }
        }

        return Name;
    }
};

////////////////////////////////////////////// RULS CLASS ////////////////////////////////////////////////////
class FuzzyRules{
public:
    vector <string> OutSymbol;
    vector<Pair> Input;
    vector<string> InSymbol;
    vector<Pair> Output;
    FuzzyRules() = default;

    void addInputRule( string name, string value) {
        Input.emplace_back(name, value);
    }

    void addInSymbol(string symbol) {
        InSymbol.push_back(symbol);
    }

    void addOutputRule(string name, string value) {
        Output.emplace_back(name, value);
    }

    void addOutSymbol(string symbol) {
        OutSymbol.push_back(symbol);
    }
};

////////////////////////////////////////////// FUZZYSYSTEM CLASS ////////////////////////////////////////////////////
class FuzzySystem {
public:
    string name;
    string description;
    vector<FuzzyVariable> Variables;
    vector<FuzzyRules> Rules;
    bool fuzzySetsAdded = false;
    double min, max;
    int ind=0;

    void Menu() {
        while (true) {
            cout << "FuzzySystem.Main Menu:\n"
                 << "==========\n"
                 << "1- Add variables.\n"
                 << "2- Add fuzzy sets to an existing variable.\n"
                 << "3- Add rules.\n"
                 << "4- Run the simulation on crisp values." << endl;
            string choice;
            cin >> choice;
            int cho;
            if (choice == "Close")break;
            else {
                char ch = choice[0];
                cho = ch - '0';
            }
            FuzzyVariable newVar;

            switch (cho) {
                case 1: {
                    cout << "Enter the variable’s name, type (IN/OUT) and range ([lower, upper]):(Press x to finish)"<< endl;
                    while (true) {
                        vector<string> info;
                        string line, ans = "";
                        getline(cin, line);
                        if (line == "x")break;
                        for (int i = 0; i < line.size(); ++i) {
                            if (line[i] == ' ' || line[i] == ',' || line[i] == '[' || line[i] == ']') {
                                info.push_back(ans);
                                ans = "";
                            } else ans += line[i];
                        }
                        if (!info.empty() && info.size() != 6) {
                            cout << "Please enter valid information" << endl;
                            continue;
                        }
                        if (!info.empty()) {
                            newVar.name = info[0];
                            if (info[1] == "IN")newVar.type = IN;
                            else if (info[1] == "OUT")newVar.type = OUT;
                            else {
                                cout << "Invalid input" << endl;
                                continue;
                            }
                            // Assuming range is in the format "[min, max]"
                            string rangeMin = info[3];
                            string rangeMax = info[5];
                            int fans = 0;
                            for (int i = 0; i < rangeMax.size(); ++i) {
                                int ans = rangeMax[i] - '0';
                                fans += ans;
                                if (i < rangeMax.size() - 1)fans *= 10;
                                // cout<<fans<<endl;
                            }
                            max = (double) fans;
                            fans = 0;
                            for (int i = 0; i < rangeMin.size(); ++i) {
                                int ans = rangeMin[i] - '0';
                                fans += ans;
                                if (i < rangeMin.size() - 1)fans *= 10;
                                // cout<<fans<<endl;
                            }
                            min = (double) fans;
                            newVar.min = min;
                            newVar.max = max;
                            Variables.push_back(newVar);
                        }
                    }
                    break;
                }
                case 2: {
                    cout << "Enter the variable’s name:" << endl;
                    cout << "--------------------------" << endl;
                    cin >> name;
                    if(IsVarExist(name)){
                        cout << "Enter the fuzzy set name, type (TRI/TRAP) and values: (Press x to finish)" << endl;
                        while (true){
                             newVar.SetMin(Variables[ind].min);
                            newVar.SetMax(Variables[ind].max);
                            string line;
                            getline(cin, line);
                            if(line == "x")break;
                            if(!line.empty()){
                                newVar.AddFuzzySet(line , name);
                                fuzzySetsAdded = true;
                            }
                        }
                    }
                    break;
                }
                case 3: {
                    cout << "Enter the rules in this format: (Press x to finish)\n"
                            "IN_variable set operator IN_variable set => OUT_variable set\n"
                            "--------------------------------------------------------------"<<endl;
                    while (true){
                        string line;
                        getline(cin, line);
                        if(line == "x")break;
                        vector<string >info = Splite(line);
                        FuzzyRules currRule;
                        int index = 0, arrow = 0;
                        for (int j = 0; j < info.size(); j++) {
                            if (info[j] == "=>") {
                                arrow = j;
                                break;
                            }
                        }
                        if(!info.empty()){
                            while (index < arrow) {
                                FuzzyVariable *currVar = GetVariableWithName(info[index]);
                                if (!IsVarExist(info[index])) {
                                    cout << "Unmatched variable name" << endl;
                                    break;
                                }
                                currRule.addInputRule(currVar->name, info[++index]);
                                index++;
                                if (index < arrow && (info[index] == "or" || info[index] == "and"
                                                      || info[index] == "or_not" || info[index] == "and_not" || info[index] == "not")) {
                                    currRule.addInSymbol(info[index]);
                                    index++;
                                }

                            }
                            //ouput
                            index = arrow + 1;
                            while (index < info.size()) {
                                FuzzyVariable *currVar = GetVariableWithName(info[index]);
                                if (!IsVarExist(info[index])) {
                                    cout << "Unmatched variable name" << endl;
                                    break;
                                }
                                //go to value
                                currRule.addOutputRule(currVar->name, info[++index]);
                                //go to optional symbol
                                index++;
                                if (index < info.size() && (info[index] == "or" || info[index] == "and"
                                                            || info[index] == "or_not" || info[index] == "and_not" || info[index] == "not")) {
                                    currRule.addOutSymbol(info[index]);
                                    index++;
                                }
                            }
                            Rules.push_back(currRule);
                            // cin >> line;
                        }
                    }
                    break;
                }
                case 4: {
                    if (Variables.size() < 1 || Rules.size() < 1 || !fuzzySetsAdded) {
                        cout << "CAN’T START THE SIMULATION! Please add the fuzzy sets and rules first." << endl;
                        break;
                    }
                    // VarType type;
                    int inVariablesSize = 0;
                    for (FuzzyVariable variable: Variables) {
                        if (variable.type == IN)
                            inVariablesSize++;
                    }
                    vector<double> values(inVariablesSize);
                    cout << "Enter the crisp values:\n"
                            "-----------------------" << endl;
                    //getting the crisp values for our variables.
                    int index = 0;
                    for (FuzzyVariable variable: Variables) {
                        if (variable.type == OUT) continue;
                        cout << variable.name << ": ";
                        double in;
                        cin >> in;
                        values[index++]=in;
                    }
                    index =0;
                    cout<<"Running the simulation..."<<endl;

                    //Fuzzification
                    for (int i = 0; i < Variables.size(); i++) {
                        if(Variables[i].type == OUT)   Variables[i].SetMembership(-1.0 , name);
                        else    Variables[i].SetMembership(values[index++] , Variables[i].name);
                    }
                    cout<<"Fuzzification => done"<<endl;

                    //Inference
                    inference();
                    cout<<"Inference => done"<<endl;

                    //Defuzzification
                    vector<string> outPut = defuzzification();
                    cout<<"Defuzzification => done"<<endl;
                    //print predicated output.
                    for (int i = 0; i < outPut.size(); ++i) {
                        cout<<outPut[i]<<" ";
                    }
                    break;
                }
                default:
                    break;

            }
        }
    }
    bool IsVarExist(string s){
        for (int i = 0; i < Variables.size(); i++) {
            if (Variables[i].name == s) {
                ind=i;
                return true;
            }
        }
        return false;
    }

    FuzzyVariable *GetVariableWithName(string s) {
        for (int i = 0; i < Variables.size(); i++) {
            if (Variables[i].name == s) {
                ind=i;
                return &Variables[i];
            }
        }
        return nullptr;
    }

    double getMembershipWithName (string s, FuzzyVariable var){
        for (int i = 0; i < memberships.size(); i++) {
            if (memberships[i].name==s) {
                return memberships[i].degreeOfMembership;
            }
        }
        return 0.0;
    }
    double getDegree (string name, string val){
        return getMembershipWithName(val, *GetVariableWithName(name));
    }
    Membership getHighestmember (FuzzyVariable var){
        Membership result = Membership("temp", -55555);
        for (Membership mem : memberships) {
            if (mem.degreeOfMembership > result.degreeOfMembership) {
                result = mem;
            }
        }
        return result;
    }

    void inference () {
        double runningValue;
        string symbol;
        for (FuzzyRules rule : Rules) {
            runningValue = getDegree(rule.Input[0].first, rule.Output[0].second);
            for (int i = 1; i < rule.Input.size(); i++) {
                symbol = rule.InSymbol[i - 1];
                if (symbol == "or") {
                    runningValue = std::max(runningValue, getDegree(rule.Input[i].first, rule.Input[i].second));
                } else if (symbol == "or_not") {
                    runningValue = 1 - std::max(runningValue, getDegree(rule.Input[i].first, rule.Input[i].second));
                } else if (symbol == "and") {
                    runningValue = std::min(runningValue, getDegree(rule.Input[i].first, rule.Input[i].second));
                } else if (symbol == "and_not") {
                    runningValue = std::min(runningValue, 1 - getDegree(rule.Input[i].first, rule.Input[i].second));
                }
            }
            FuzzyVariable* variablePtr = GetVariableWithName(rule.Output[0].first);

// Check if the variable pointer is not null
            if (variablePtr != nullptr) {
                // Access the memberships vector
                vector<Membership>& members = memberships;

                // Iterate over the memberships vector
                for (int i = 0; i < members.size(); i++) {
                    Membership& member = members[i];

                    // Compare names case-insensitively
                    if (member.name== rule.Output[0].second) {
                        // Update the degreeOfMembership if runningValue is greater
                        if (runningValue > member.degreeOfMembership) {
                            member.degreeOfMembership = runningValue;
                        }
                        break;
                    }
                }
            } else {
                // Handle the case where the variable is not found
                cout << "Variable not found." << endl;
            }
        }
    }
    vector<double> calculateCenter( FuzzyVariable var) {
        vector<double> result;

        for ( FuzzySet set : fuzzySets) {
            double sum = 0.0;
            for (int j = 0; j < set.values.size(); ++j) {
                sum += set.values[j];
            }

            result.push_back(sum / set.values.size());
        }

        return result;
    }
    double getPredict(FuzzyVariable var) {
        double result = 0.0;
        double divided = 0.0;

       vector<double> center = calculateCenter(var);

        for (size_t i = 0; i < center.size(); ++i) {
            result += center[i] * memberships[i].degreeOfMembership;
            divided += memberships[i].degreeOfMembership;
        }

        if (divided != 0.0) {
            result /= divided;
        } else {
            result = 0.0;
        }
        return result;
    }

    vector< string> defuzzification() {
       vector< string> s;
        for ( FuzzyVariable var : Variables) {
            if (var.type == OUT) {
                var.SetMembership(Approximate(getPredict(var)),var.name);
                s.push_back("The predicted ");
                s.push_back(var.name);
                s.push_back(" is ");
                s.push_back(var.GetMaxMember());
                 s.push_back(" (");
                 s.push_back(to_string(getPredict(var)));
                 s.push_back(")\n");
            }
        }
        return s;
    }

    double Approximate(double getPredict) {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(1) << getPredict;
        return std::stod(stream.str());
    }

};

int main()
{
    FuzzyVariable Variable;
    int choice;
    cout << "Fuzzy Logic Toolbox\n"
         << "===================\n"
         << "1- Create a new fuzzy system\n"
         << "2- Quit" << endl;
    cin>>choice;
    if(choice==1){
        cout<<"Enter the system’s name and a brief description:\n"
            <<"------------------------------------------------"<<endl;
        string Dis,Name;
        getline(cin,Dis);
        FuzzySystem fuzzySystem;
        fuzzySystem.Menu();
    }
    else return 0;
}
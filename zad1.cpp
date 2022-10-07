//
// Created by s22785 on 05.10.2022.
//

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>

using my_fun = std::function<double(std::vector<double>)>;

void lab1(std::vector<double> num, my_fun fun){
    using namespace std;
        cout << fun(num) << endl;
}

int main(int argc, char **argv) {
    using namespace std;
    map<int, int> map1 = {{}};
    map<string, my_fun> format;
    format["add"] = [](vector<double> num) { return num.front() + num.back(); };
    format["mod"] = [](vector<double> num) { return (int)num.front() % (int)num.back(); };
    format["sin"] = [](vector<double> num) { return sin(num.front()); };


    try {
        vector<string> argument(argv, argv + argc);
        auto selected_f = argument.at(2);
        if(argument.size() > 5){
            cout << "za wiele argumentow" <<endl;
            return 1;
        }
        else if(selected_f == "sin" && argument.size() > 4){
            cout << "za wiele argumentow" <<endl;
            return 1;
        }
        else if(argument.at(1)!="lab1"){
            cout << "na poczatku powinno pojawic sie 'lab1'" <<endl;
            return 1;
        }

        vector<double> num = {{stod(argument.at(3)), stod(argument.back())}};
        lab1(num, format.at(selected_f));
    } catch (std::out_of_range aor) {
        cout << "podaj argument. Dostepne to: ";
        for (auto [k, v] : format) cout <<endl << " " << k;
        cout << endl;
    }
    return 0;

}


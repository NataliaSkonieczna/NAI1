//
// Created by s22785 on 05.10.2022.
//

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using my_fun = std::function<int(int)>;
using my_map = std::map<int, int>;

void lab1(my_map map, my_fun){
    using namespace std;

        cout << "" << endl;

}

int main(int argc, char **argv) {
    using namespace std;
    map<int, int> map1 = {{}};
    map<string, my_fun> format;
    format["add"] = [](int x) { return x; };
    format["mod"] = [](int x) { return x; };
    format["sin"] = [](int x) { return x; };

    try {
        vector<string> argumenty(argv, argv + argc);
        auto selected_f = argumenty.at(1);
        lab1(map1, format.at(selected_f));
    } catch (std::out_of_range aor) {
        cout << "podaj argument. Dostepne to: ";
        for (auto [k, v] : format) cout <<endl << " " << k;
        cout << endl;
    }
    return 0;

}


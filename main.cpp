#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <math.h>

using domain_t = std::vector<double>;
std::random_device rd;
std::mt19937 mt_generator(rd());
std::ofstream outfile("file.txt");
using myfun = std::function<double(double, double)>;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

double hill_climbing(std::function<double(double, double)> &f,domain_t minimal_d, domain_t maximal_d, int max_iterations) {

    int times= max_iterations/1;

    auto t1 = high_resolution_clock::now();
    std::uniform_real_distribution<double> dist(minimal_d[0], maximal_d[0]);
    double current_p = f(minimal_d[0], maximal_d[0]);
    double new_p(minimal_d.size());
    double new_p2(minimal_d.size());


    for (int iteration = 0; iteration < max_iterations; iteration++) {

        new_p = dist(mt_generator);
        new_p2 = dist(mt_generator);

        if (f(new_p,new_p2) < current_p){
            current_p = f(new_p,new_p2);
        }

        if(iteration % times == 0){
            auto t2 = high_resolution_clock::now();
            auto ms_int = duration_cast<milliseconds>(t2 - t1);
            duration<double, std::milli> ms_double = t2 - t1;
            std::cout << std::endl << "time = " << ms_double.count() << "ms\n";

            outfile.open("file.txt",std::ios_base::app);
            outfile<<ms_double.count()<< " " << current_p << std::endl;
            outfile.close();
        }
    }

    std::cout << "best x = ";
    return current_p;
}


auto brute_force = [](std::function<double(double, double)> &f, domain_t minimal_d, domain_t maximal_d, int max_iterations) {

    auto t1 = high_resolution_clock::now();
    std::uniform_real_distribution<double> dist(minimal_d[0], maximal_d[0]);

    auto current_p = dist(mt_generator);
    auto current_p2 = dist(mt_generator);
    auto best_point = f(current_p,current_p2);

        for(int i=0;i<max_iterations;i++) {
            if (f(current_p,current_p2) < best_point) {
                best_point = f(current_p,current_p2);
            }
            current_p = dist(mt_generator);
            current_p2 = dist(mt_generator);
        }
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << std::endl << "time = " << ms_double.count() << "ms\n";
    return best_point;
};

double simulatedAnnealing(std::function<double(double, double)> &f,domain_t minimal_d, domain_t maximal_d, int max_iterations) {

    auto t1 = high_resolution_clock::now();

    std::uniform_real_distribution<double> uk(0, 1);
    double u = uk(mt_generator);

    double s = f(minimal_d[0], maximal_d[0]);
    double s2 = f(minimal_d[0], maximal_d[0]);
    std::vector<double> myvec;
    myvec.push_back(s);


    for (int iterations = 0; iterations < max_iterations; ++iterations) {
        double tk = f(minimal_d[0], maximal_d[0]);
        double tk2 = f(minimal_d[0], maximal_d[0]);

        if (f(tk, tk2) < f(s, s2)) {
            s =  f(tk, tk2);
            myvec.push_back(s);
        } else {
            if (u < exp(-(fabs(f(tk, tk2) - f(s, s2))/((1/log(iterations)))))) {
                s =  f(tk, tk2);
                myvec.push_back(s);
            }
        }

    }
    for(int j=0;myvec.size()>=j;j++) {
        if (myvec[j] < s) {
            s = myvec[j];
        }
    }

    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << std::endl << "time = " << ms_double.count() << "ms\n";
    return s;
}

    std::function<double(double, double)> beale = [](double x, double y) {
        return pow(1.5 - x + (x * y), 2) + pow(2.25 - x + x * pow(y, 2), 2) + pow(2.625 - x + (x * pow(y, 3)), 2);
    };
    std::function<double(double, double)> cross = [](double x, double y) {
        return -0.0001*(pow((fabs(sin(x)*sin(y)*exp(fabs(100-(sqrt(pow(x,2)+pow(y,2)))/M_PI))+1)),0.1));
    };
    std::function<double(double, double)> himmelblau = [](double x, double y) {
        return pow(pow(x,2)+y-11,2)+pow(x+pow(y,2)-7,2);
    };

void hillClimbCout () {
    std::cout << hill_climbing(beale, {-4.5}, {4.5}, 10000) << std::endl;
    std::cout << hill_climbing(cross, {-10}, {10}, 10000) << std::endl;
    std::cout << hill_climbing(himmelblau, {-5}, {5}, 10000) << std::endl;
}

void bruteForceCout(){
    std::cout << brute_force(beale, {-4.5}, {4.5},10000) << std::endl;
    std::cout << brute_force(cross, {-10}, {10},10000) << std::endl;
    std::cout << brute_force(himmelblau, {-5}, {5}, 10000) << std::endl;

}

void simulatedAnnealingCout(){
    std::cout << simulatedAnnealing(beale, {-4.5}, {4.5}, 10000) << std::endl;
    std::cout << simulatedAnnealing(cross, {-10}, {10}, 10000) << std::endl;
    std::cout << simulatedAnnealing(himmelblau, {-5}, {5}, 10000) << std::endl;

}

int main(int argc, char **argv) {

    outfile.open("file.txt",std::ios_base::app);
    outfile<<std::endl;
    outfile.close();

    std::string array[3] = {"1 Hill Climb","2 Brute Force","3 Simulated Annealing"};

    try {
        std::vector<std::string> argument(argv, argv + argc);
        auto selected_f = argument.at(1);

        switch(std::stoi(argument.at(1))){
            case 1:
                std::cout<<"Hill Climb"<<std::endl;
                hillClimbCout();
                break;
            case 2:
                std::cout<<"Brute Force"<<std::endl;
                bruteForceCout();
                break;
            case 3:
                std::cout<<"Simulated Annealing"<<std::endl;
                simulatedAnnealingCout();
                break;
        }

    } catch (std::out_of_range aor) {
        std::cout << "podaj argument. Dostepne to: "<<std::endl;
        for (int i=0;i<=2;i++) std::cout<< array[i]<<std::endl;
        std::cout << std::endl;
    }



//brute force dziala
//hill climb dziala
//anne nie dziala ?????

}

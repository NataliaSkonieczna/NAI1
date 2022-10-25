#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <chrono>
#include <fstream>
#include <map>
#include <string>
#include <tgmath.h>
#include <math.h>
/**
 * domain - generate domain points. Throws exception when all the points were returned
 */

/* f -> funkcja, domain ->dziedzina*/


using domain_t = std::vector<double>;
std::random_device rd;
std::mt19937 mt_generator(rd());
std::ofstream outfile("file.txt");
using myfun = std::function<double(double, double)>;

auto brute_force = [](std::function<double(double, double)> &f, domain_t minimal_d, domain_t maximal_d, int max_iterations) {

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
    return best_point;
};



double simulatedAnnealing(std::function<double(double, double)> &f,domain_t minimal_d, domain_t maximal_d, int max_iterations, double temperature) {

    std::uniform_real_distribution<double> uk(0, 1);
    double u = uk(mt_generator);

    double s = f(minimal_d[0], maximal_d[0]);
    double s2 = f(minimal_d[0], maximal_d[0]);
    std::vector<std::pair<double,double>> myvec;


    for (int iterations = 1; iterations < max_iterations; ++iterations) {
        double tk = f(minimal_d[0], maximal_d[0]);
        double tk2 = f(minimal_d[0], maximal_d[0]);

        if (f(tk, tk2) < f(s, s2)) {
            s =  f(tk, tk2);
        } else {
            if (u < exp(-(fabs(f(tk, tk2) - f(s, s2)) / ((1 / log(iterations))*temperature)))) {
                s =  f(tk, tk2);
            }
        }
    }
    return s;
}

double hill_climbing(std::function<double(double, double)> &f,domain_t minimal_d, domain_t maximal_d, int max_iterations) {

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;


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

    std::function<double(double, double)> beale = [](double x, double y) {
        return pow(1.5 - x + (x * y), 2) + pow(2.25 - x + x * pow(y, 2), 2) + pow(2.625 - x + (x * pow(y, 3)), 2);
    };
    std::function<double(double, double)> cross = [](double x, double y) {
        return -0.0001*(pow((fabs(sin(x)*sin(y)*exp(fabs(100-(sqrt(pow(x,2)+pow(y,2)))/M_PI))+1)),0.1));
    };
    std::function<double(double, double)> matyas = [](double x, double y) {
        return 0.26 * ((x * x) + (y * y)) - 0.48 * x * y;
    };

void hillClimbCout () {
    std::cout << hill_climbing(beale, {-4.5}, {4.5}, 10000) << std::endl;
    std::cout << hill_climbing(cross, {-10}, {10}, 10000) << std::endl;
    std::cout << hill_climbing(matyas, {-4.5}, {4.5}, 10000) << std::endl;
}

void simulatedAnnealingCout(){
    std::cout << simulatedAnnealing(beale, {-4.5}, {4.5}, 10000,4) << std::endl;
    std::cout << simulatedAnnealing(cross, {-10}, {10}, 10000,4) << std::endl;
    std::cout << simulatedAnnealing(matyas, {-4.5}, {4.5}, 10000,4) << std::endl;

}

void bruteForceCout(){
    std::cout << brute_force(beale, {-4.5}, {4.5},10000) << std::endl;
    std::cout << brute_force(cross, {-10}, {10},10000) << std::endl;
    std::cout << brute_force(matyas, {-4.5}, {4.5},10000) << std::endl;

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
                hillClimbCout();
                break;
            case 2:
                bruteForceCout();
                break;
            case 3:
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
//anne nie dziala

}

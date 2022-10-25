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
auto brute_force = [](auto f, auto domain) {

    auto current_p = domain();
    auto best_point = current_p;
    try {
        while (true) {
            if (f(current_p) < f(best_point)) {
                best_point = current_p;
            }
            current_p = domain();
        }
    } catch (std::exception &e) {
    }
    return best_point;
};


using domain_t = std::vector<double>;
std::random_device rd;
std::mt19937 mt_generator(rd());
std::ofstream outfile("file.txt");
using myfun = std::function<double(double, double)>;


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
            s = tk;
        } else {
            if (u < exp(-(fabs(f(tk, tk2) - f(s, s2)) / ((1 / log(iterations))*temperature)))) {
                s = tk;
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
    std::cout << hill_climbing(cross, {-4.5}, {4.5}, 10000) << std::endl;
    std::cout << hill_climbing(matyas, {-4.5}, {4.5}, 10000) << std::endl;
}

void simulatedAnnealingCout(){

    std::cout << simulatedAnnealing(beale, {-4.5}, {4.5}, 10000,4) << std::endl;
    std::cout << simulatedAnnealing(cross, {-4.5}, {4.5}, 10000,4) << std::endl;
    std::cout << simulatedAnnealing(matyas, {-4.5}, {4.5}, 10000,4) << std::endl;

}

int main(int argc) {

    outfile.open("file.txt",std::ios_base::app);
    outfile<<std::endl;
    outfile.close();

    simulatedAnnealingCout();

}

#include <iostream>
#include <vector>
#include <functional>
#include <random>
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

double hill_climbing(std::function<double(double, double)> &f,domain_t minimal_d, domain_t maximal_d, int max_iterations=1000) {
    double current_p(minimal_d.size());
    double current_p_n(minimal_d.size());
    for (int i = 0; i < minimal_d.size(); i++) {
        std::uniform_real_distribution<double> dist(minimal_d[i], maximal_d[i]);
        current_p = dist(mt_generator);
        current_p_n = dist(mt_generator);
    }
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        double new_p(minimal_d.size());

        for (int i = 0; i < minimal_d.size(); i++) {
            std::uniform_real_distribution<double> dist(-1.0/128.0, 1.0/128.0);
            new_p = current_p + dist(mt_generator);

        }
        if (f(current_p,current_p_n) > new_p ){
            current_p = new_p;
        }
    }
    return current_p;
}


int main() {

    auto sphere_f = [](double x) {return x*x;};
    double current_sphere_x = -10;
    auto sphere_generator = [&]() {
        current_sphere_x += 1.0/128.0;
        if (current_sphere_x >= 10) throw std::invalid_argument("finished");
        return current_sphere_x;
    };
    auto best_point = brute_force(sphere_f, sphere_generator);
    std::cout << "best x = " << best_point << std::endl;


    std::function<double(double,double)> beale = [](double x, double y){return pow(1.5-x+x*y,2) + pow(2.25-x+sqrt(x*y),2)+pow(2.625-x+std::pow(x*y, 1/3.),2);};
    std::cout << "best x = " << hill_climbing(beale,{-4.5},{4.5},10000) << std::endl;

    std::function<double(double,double)> booth = [](double x, double y){return pow(x+2*y-7,2)+pow(2*x+y-5,2);};
    std::cout << "best x = " << hill_climbing(booth,{-4.5},{4.5},10000) << std::endl;

    std::function<double(double,double)> matyas = [](double x, double y){return 0.26*((x*x)+(y*y))-0.48*x*y;};
    std::cout << "best x = " << hill_climbing(matyas,{-4.5},{4.5},10000) << std::endl;




    return 0;
}
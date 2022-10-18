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

domain_t hill_climbing(const std::function<double(domain_t)> &f, domain_t minimal_d, domain_t maximal_d, int max_iterations) {
    domain_t current_p(minimal_d.size());
    for (int i = 0; i < minimal_d.size(); i++) {
        std::uniform_real_distribution<double> dist(minimal_d[i], maximal_d[i]);
        current_p[i] = dist(mt_generator);
    }
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        domain_t new_p(minimal_d.size());
        for (int i = 0; i < minimal_d.size(); i++) {
            std::uniform_real_distribution<double> dist(-1.0/128.0, 1.0/128.0);
            new_p[i] = current_p[i] + dist(mt_generator);
        }
        if (f(current_p) > f(new_p)) {
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

    auto beale = [](domain_t x) {return sqrt(1.5-x[0]+x[0]*x[1]) + sqrt(2.25-x[0]+sqrt(x[0]*x[1]))+sqrt(2.625-x[0]+cbrt(x[0]*x[1]));};
    auto best2 = hill_climbing(beale, {-4.5},{4.5},10000);
    std::cout << "best x = " << best2[0] << std::endl;


    auto booth = [](domain_t x) {return sqrt(x[0]+2*x[0]-7)+ sqrt(2*x[0]+x[1]-5);};
    auto best3 = hill_climbing(beale, {-10},{10},10000);
    std::cout << "best x = " << best3[0] << std::endl;


    auto matyas = [](domain_t x) {return 0.26*(sqrt(x[0])+ sqrt(x[1])-0.48*x[0]*x[1]);};
    auto best4 = hill_climbing(matyas, {-10},{10},10000);
    std::cout << "best x = " << best4[0] << std::endl;


    return 0;
}
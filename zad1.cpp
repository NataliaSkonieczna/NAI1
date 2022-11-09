#include <iostream>
#include <vector>
#include <functional>
#include <random>
std::random_device rd;
std::mt19937 mt_generator(rd());
using myfun = std::function<double(double, double)>;

auto genetic_algorithm = [](
        auto initial_population, auto fitness, auto term_condition,
        auto selection, double p_crossover,
        auto crossover, double p_mutation,  auto mutation) {
    using namespace std;
    uniform_real_distribution<double> uniform(0.0,1.0);
    auto population = initial_population;
    vector<double> population_fit = fitness(population);
    while (!term_condition(population,population_fit)) {
        auto parents_indexes = selection(population_fit);
        decltype(population) new_population;
        for (int i = 0 ; i < parents_indexes.size(); i+=2) {
            decltype(initial_population) offspring = {population[i],population[i+1]};
            if (uniform(mt_generator) < p_crossover) {
                offspring = crossover(offspring);
            }
            for (auto chromosome : offspring) new_population.push_back(chromosome);
        }
        for (auto & chromosome : new_population) {
            chromosome = mutation(chromosome,p_mutation);
        }
        population = new_population;
        population_fit = fitness(population);
    }
    return population;
};

using chromosome_t = std::vector<int>;
using population_t = std::vector<chromosome_t>;

std::pair<double,double> translate(chromosome_t chromosome){
    int size = chromosome.size();
    double x = 0.0;
    double y = 0.0;

    for(int i=0;i<size/2;i++){
        x = x+ (chromosome[i]*pow(2,i));
    }
    for(int j=size/2; j<size;j++){
        y=y+(chromosome[j]*pow(2,j));
    }

    return{x,y};
}

population_t populate (int size, int chromLen){
    population_t population1;
    for(int i=0; i<size ;i++){
        chromosome_t chromosome;
        for(int j=0; j<chromLen;j++){
            chromosome.push_back(rand()%2);
        }
       population1.push_back(chromosome);
    }
    return population1;
}

std::vector<double> fintess_function(population_t pop){


    return {};
}


std::vector<int> selection_empty(std::vector<double> fitnesses) {
    return {};
}
std::vector<chromosome_t > crossover_empty(std::vector<chromosome_t > parents) {
    return parents;
}
chromosome_t mutation_empty(chromosome_t parents, double p_mutation) {
    return parents;
}
int main() {

    std::function<double(double, double)> beale = [](double x, double y) {
        return pow(1.5 - x + (x * y), 2) + pow(2.25 - x + x * pow(y, 2), 2) + pow(2.625 - x + (x * pow(y, 3)), 2);
    };
    std::function<double(double, double)> cross = [](double x, double y) {
        return -0.0001*(pow((fabs(sin(x)*sin(y)*exp(fabs(100-(sqrt(pow(x,2)+pow(y,2)))/M_PI))+1)),0.1));
    };
    std::function<double(double, double)> himmelblau = [](double x, double y) {
        return pow(pow(x,2)+y-11,2)+pow(x+pow(y,2)-7,2);
    };

    using namespace std;
    population_t population = populate(100,100+((22785%10)*2));
    auto result = genetic_algorithm(population,
                                    fintess_function,
                                    [](auto a, auto b){return true;},
                                    selection_empty, 1.0,
                                    crossover_empty,
                                    0.01, mutation_empty);
    /*for (chromosome_t chromosome: result) {
        cout << "[";
        for (int p: chromosome) {
            cout << p;
        }
        cout << "] ";
    }*/

    cout << endl;
    return 0;
}

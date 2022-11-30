#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <map>
std::random_device rd;
std::mt19937 mt_generator(rd());
using myfun = std::function<double(std::pair<double, double>)>;
using car = std::function<double>;

std::vector<double> populationCheck(std::vector<double> fitness){
    double min = fitness.at(0);
    double max = 0;
    double temp = 0;

    for (auto fit : fitness){
        temp = temp + fit;
        if(fit > max){
            max = fit;
        }
        if (fit < min){
            min = fit;
        }
    }

    double avg = temp/fitness.size();

    std::vector<double> populationCheck;

    populationCheck.push_back(min);
    populationCheck.push_back(max);
    populationCheck.push_back(avg);

    return populationCheck;
}

auto genetic_algorithm = [](
        auto initial_population, auto fitness, auto term_condition,
        auto selection, double p_crossover,
        auto crossover, double p_mutation,  auto mutation, auto myfun, auto domain,int maxIter,bool print) {
    using namespace std;
    uniform_real_distribution<double> uniform(0.0,1.0);
    auto population = initial_population;
    int iteration = 0;
    vector<double> population_fit = fitness(population,myfun,domain,print,iteration);
    while (!term_condition(population,population_fit,iteration,maxIter)) {
            std::cout<< "ITERACJE : " << iteration<< std::endl;
            auto parents_indexes = selection(population_fit);
            decltype(population) new_population;
            for (int i = 0; i < parents_indexes.size(); i += 2) {
                decltype(initial_population) offspring = {population[i], population[i + 1]};
                if (uniform(mt_generator) < p_crossover) {
                    offspring = crossover(offspring);
                }
                for (auto chromosome: offspring) new_population.push_back(chromosome);
            }
            for (auto &chromosome: new_population) {
                chromosome = mutation(chromosome, p_mutation);
            }
            population = new_population;
            population_fit = fitness(population, myfun, domain, print, iteration);
            iteration++;
    }
    return population;
};

using chromosome_t = std::vector<int>;
using population_t = std::vector<chromosome_t>;

std::pair<double,double> translate(chromosome_t chromosome,std::vector<double> domain){
    int size = chromosome.size();
    double x = 0.0;
    double y = 0.0;

    for (int i = 1; i < size / 2; i++) {
        x += chromosome[i] * pow(2, -i);
    }

    for (int i = size / 2 + 1; i < size; i++) {
        y += chromosome[i] * pow(2, -(i - (size / 2)));
    }

    if (chromosome[0] == 1) {
        x = -1.0;
    }
    if (chromosome[size / 2] == 1) {
        y= -1.0;
    }

    return{x*std::abs(domain[0]),y*std::abs(domain[1])};
}

population_t populate (int size, int chromLen){
    population_t population1;
    std::uniform_int_distribution<int> rnd(0, 1);
    for(int i=0; i<size ;i++){
        chromosome_t chromosome;
        for(int j=0; j<chromLen;j++){
            chromosome.push_back(rnd(mt_generator));
        }
        population1.push_back(chromosome);
    }
    return population1;
}

std::vector<double> fintess_function(population_t pop,myfun fun,std::vector<double> domain,bool print, int iterations) {
    std::vector<double> temp;
    std::pair<double, double> vec;
    for (int i=0; i < pop.size(); i++) {
        vec = translate(pop[i],domain);
        if (vec.first > domain.at(0) && vec.second > domain.at(0) && vec.first < domain.at(1) &&
            vec.second < domain.at(1)) {
            temp.push_back(1000 - fun(vec));
        }
        else{
            temp.push_back(0);
        }
    }
    if (print){
        std::vector<double> printPop = populationCheck(temp);
        std::cout << "min: " << printPop.at(0) << std::endl;
        std::cout << "max: " << printPop.at(1) << std::endl;
        std::cout << "avg: " << printPop.at(2) << std::endl;
    }
        return temp;
    }

std::vector<int> selection_empty(std::vector<double> fitnesses) {
    double S=0;
    double P=0;
    double P1=0;
    double pi=0;
    double pi1=0;
    std::vector<int> vec;
    std::uniform_real_distribution<> rnd(0,1);
    double R = rnd(mt_generator);

    for(int e : fitnesses){
        S+=e;
    }
    for (int i = 0; i < fitnesses.size(); i++) {
        pi = fitnesses.at(i) / S;
        P = P1 + pi;
        if(P1 <= R && pi1 <= P){
            vec.push_back(i);
        }
        P1 = P;
    }
    return vec;
}
std::vector<chromosome_t > crossover_empty(std::vector<chromosome_t > parents) {
    std::uniform_real_distribution<double> S(0,parents.at(0).size());
    int rand = S(mt_generator);
    for(int i = rand; i < parents.at(0).size();i++){
        int index = parents.at(0).at(i);
        parents.at(0).at(i) = parents.at(1).at(i);
        parents.at(1).at(i) = index;
    }
    return parents;
}
chromosome_t mutation_empty(chromosome_t parents, double p_mutation) {
    std::uniform_real_distribution<> randNum(0,1);
    std::uniform_real_distribution<> randQuan(0,8);
    std::uniform_real_distribution<> randPoint(0,parents.size());
    int Quantity = randQuan(mt_generator);
    if(randNum(mt_generator)<p_mutation){
        for(int i = Quantity ; i>0 ; i--){
            int temp = randPoint(mt_generator);
            parents.at(temp) = randNum(mt_generator);
        }
    }
    return parents;
}
int main(int argc, char **argv) {

    std::map<std::string, std::vector<double>> domain;

    std::function<double(std::pair<double, double>)> beale = [](std::pair<double, double> xy) {
        return pow(1.5 - xy.first + (xy.first * xy.second), 2) + pow(2.25 - xy.first + xy.first * pow(xy.first, 2), 2) + pow(2.625 - xy.first + (xy.first * pow(xy.second, 3)), 2);
    };
    std::function<double(std::pair<double, double>)> cross = [](std::pair<double, double> xy) {
        return -0.0001*(pow((fabs(sin(xy.first)*sin(xy.second)*exp(fabs(100-(sqrt(pow(xy.first,2)+pow(xy.second,2)))/M_PI))+1)),0.1));
    };
    std::function<double(std::pair<double, double>)> himmelblau = [](std::pair<double, double> xy) {
        return pow(pow(xy.first,2)+xy.second-11,2)+pow(xy.first+pow(xy.second,2)-7,2);
    };

    domain["beale"] = {-4.5,4.5};
    domain["cross"] = {-10,10};
    domain["himmelblau"] = {-5,5};

    using namespace std;
    population_t population = populate(100,100+((22785%10)*2));

    /*try {
        auto result = genetic_algorithm(population,
                                        fintess_function,
                                        [](auto a, auto b) {
                                            for (auto count: b) {
                                                if(count > 950){
                                                    return true;
                                                }
                                            }
                                            return false;
                                        },
                                        selection_empty, 1.0,
                                        crossover_empty,
                                        0.01, mutation_empty, himmelblau, domain["himmelblau"],10);

        for (chromosome_t chromosome: result) {
            cout << "[";
            for (int p: chromosome) {
                cout << p;
            }
            cout << "] " << endl;
        }

    }

    catch(std::out_of_range aor) {
        return 1;
    }*/



        std::vector<std::string> argument(argv, argv + argc);
        auto popSize = argument.at(1);
        auto iterations = argument.at(2);
        auto crossPoss = argument.at(3);
        auto mutPoss = argument.at(4);
        auto print = argument.at(5);
        bool print2 = false;

        if (print == "true"){
            print2 = true;
        }if(print == "false"){
            print2 = false;
        }

        population_t population2 = populate(stoi(popSize),100+((22785%10)*2));

        auto result = genetic_algorithm(population2,fintess_function,[](auto a, auto b, auto iterations, auto maxIterations) {
                                        return iterations > maxIterations;
                                        },selection_empty, stod(crossPoss),
                                        crossover_empty,
                                        stod(mutPoss), mutation_empty, himmelblau, domain["himmelblau"],stoi(iterations),print2);







    return 0;
}
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

// Khoi tao so thanh pho trong bai toan
const int numCities = 20;

// Khoi tao thanh pho voi 2 toa do x va y
struct City {
    int x;
    int y;
};

// Khoang cach giua 2 thanh pho
double distance(City a, City b) {
    return sqrt(pow(a.x-b.x, 2) + pow(a.y-b.y, 2));
}

// Tao ra cac gia tri nguyen hoan vi tu vi tri 0 -> n-1
vector<int> randomPermutation(int n) {
    vector<int> perm(n);
    for (int i=0; i<n; i++) {
        perm[i] = i;
    }
    random_shuffle(perm.begin(), perm.end());
    return perm;
}

// Tao ra quan the ban dau cua tour du lich
vector<vector<int> > initialPopulation(int popSize) {
    vector<vector<int> > population(popSize);
    for (int i=0; i<popSize; i++) {
        population[i] = randomPermutation(numCities);
    }
    return population;
}

// Tinh tong do dai duong di thich hop cua ca tour du lich/Calculate fitness of a tour as the total distance traveled
double tourDistance(vector<int> tour, vector<City> cities) {
    double dist = 0.0;
    for (int i=0; i<numCities-1; i++) {
        dist += distance(cities[tour[i]], cities[tour[i+1]]);
    }
    dist += distance(cities[tour[numCities-1]], cities[tour[0]]);
    return dist;
}

// Chon ra 2 ca the phu huynh trong quan the su dung thuat toan touarnament selection/Select two parents from the population using tournament selection
pair<vector<int>, vector<int> > tournamentSelection(vector<vector<int> > population, vector<City> cities) {
    int tournamentSize = 5;
    int idx1 = rand() % population.size();
    for (int i=0; i<tournamentSize-1; i++) {
        int idx2 = rand() % population.size();
        if (tourDistance(population[idx2], cities) < tourDistance(population[idx1], cities)) {
            idx1 = idx2;
        }
    }
    int idx2 = rand() % population.size();
    for (int i=0; i<tournamentSize-1; i++) {
        int idx3 = rand() % population.size();
        if (idx3 != idx1 && tourDistance(population[idx3], cities) < tourDistance(population[idx2], cities)) {
            idx2 = idx3;
        }
    }
    return make_pair(population[idx1], population[idx2]);
}

// Hàm lai ghép (crossover) giua 2 cá th? phu huynh de tao ra the he sau/ Crossover two parents to produce a child
vector<int> crossover(vector<int> parent1, vector<int> parent2) {
    int start = rand() % numCities;
    int end = rand() % numCities;
    if (start > end) {
        swap(start, end);
    }
    vector<int> child(numCities, -1);
    for (int i=start; i<=end; i++) {
        child[i] = parent1[i];
    }
    int j = 0;
    for (int i=0; i<numCities; i++) {
        if (child[i] == -1) {
            while (find(child.begin(), child.end(), parent2[j]) != child.end()) {
                j++;
            }
            child[i] = parent2[j];
        }
    }
    return child;
}

// Dot bien (Mutation) chuyen du lich bang cach trao doi hoan doi vi tri giua 2 thanh pho/ Mutate a tour by swapping two cities
void mutate(vector<int>& tour) {
int idx1 = rand() % numCities;
int idx2 = rand() % numCities;
swap(tour[idx1], tour[idx2]);
}

// Khoi tao mot quan the the moi tu quan the cu su dung elitism, tournamen selection, cross over va mutation / Create a new population from the old population using elitism, tournament selection, crossover, and mutation
vector<vector<int> > newPopulation(vector<vector<int> > oldPopulation, vector<City> cities) {
vector<vector<int> > newPopulation(oldPopulation.size());
// Giu lai duong di tot nhat tu quan the cu / Keep the best tour from the old population
int bestIdx = 0;
double bestDist = tourDistance(oldPopulation[0], cities);
for (int i=1; i<oldPopulation.size(); i++) {
	double dist = tourDistance(oldPopulation[i], cities);
	if (dist < bestDist) {
		bestIdx = i;
		bestDist = dist;
	}
}
newPopulation[0] = oldPopulation[bestIdx];
// Tao ra duong di moi bang cach su dung lai ghep (crossover) va dot bien (mutation)/Create new tours using crossover and mutation
for (int i=1; i<oldPopulation.size(); i++) {
pair<vector<int>, vector<int> > parents = tournamentSelection(oldPopulation, cities);
vector<int> child = crossover(parents.first, parents.second);
mutate(child);
newPopulation[i] = child;
}
return newPopulation;
}

// Tim ra duong di tot nhat trong quan the duong di / Find the best tour in the population
vector<int> bestTour(vector<vector<int> > population, vector<City> cities) {
int bestIdx = 0;
double bestDist = tourDistance(population[0], cities);
for (int i=1; i<population.size(); i++) {
	double dist = tourDistance(population[i], cities);
	if (dist < bestDist) {
	bestIdx = i;
	bestDist = dist;
	}	
}
return population[bestIdx];
}

int main() {
// Seed trinh tao ra so ngau nhien
	srand(time(NULL));
// Khoi tao ngau nhien cac gia tri toa do cua cac thanh pho
	vector<City> cities(numCities);
	for (int i=0; i<numCities; i++) {
		cities[i].x = rand() % 100;
		cities[i].y = rand() % 100;
	}
// Dua ra man hinh toa do cua thanh pho
	for (int i=0;i<20;i++){
		cout<<"City "<<i<<" x,y : "<<cities[i].x<< " "<<cities[i].y<<endl;
	}
	cout<<endl;
// Khoi tao quan the
	int popSize = 100;
	vector<vector<int> > population = initialPopulation(popSize);
// Chay giai thuat di chuyen trong mot gioi han the he (nhu o duoi la di truyen trong 1000 the he)/Run the GA for a fixed number of generations
	int numGenerations = 1000;
	for (int i=0; i<numGenerations; i++) {
		population = newPopulation(population, cities);
	}
// Goi lai cac ham da lap o tren de tim ra duong di ngan nhat, lo trinh va do dai cua no / Find the best tour and print its distance
	vector<int> tour = bestTour(population, cities);
	
	cout<<"Lo trinh duong di ngan nhat: ";
 	for (int i=0;i<20;i++) cout<<tour[i]<<"-->";
 	cout<<tour[0]<<endl;
 	
	double dist = tourDistance(tour, cities);
	cout<<endl;
	cout << "Do dai duong di ngan nhat: " << dist << endl;
	return 0;
}

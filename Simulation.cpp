#include <iostream>
#include <vector>
#include <math.h>
#include <chrono>
#include <fstream>
using namespace std;

double randomGenerator(long=11);
double mean(vector<long long>);
double variance(vector<long long>);

class Coin
{
private:
    double prob;
public:
    Coin(double p) {
        prob = p;
    }

    bool flip(){
        return randomGenerator() < prob;
    }
};

int main()
{
    auto p = 0.0001;
    int numberOfTrials = 1000;
    vector<double> Ns = {1e3, 1e4, 2e4, 5e4, 1e5, 2e5, 3e5, 4e5, 5e5, 6e5, 7e5, 8e5, 9e5, 1e6};

    ofstream variances;
    variances.open("/home/saleh/Projects/Data-simulation/Variances.txt");

    ofstream means;
    means.open("/home/saleh/Projects/Data-simulation/Means.txt");

    Coin myCoin{p};

    auto starting = chrono::steady_clock::now();
    for (auto N : Ns) {
        auto start_inside = chrono::steady_clock::now();

        cout << "Strating iteration for N = " << N << "..." << endl;

        vector<long long> result{};

        for (int _ = 0; _ < numberOfTrials; ++_) {
            long long localRes{0};

            for (int iteration = 0; iteration < N; ++iteration)
                if (myCoin.flip())
                    ++localRes;

            result.push_back(localRes);
        }

        means << mean(result) << ' ';
        variances << variance(result) << ' ';

        cout << "Done!\n";

        auto end_inside = chrono::steady_clock::now();
        cout << "Elapsed time: " << chrono::duration_cast<chrono::nanoseconds>(end_inside - start_inside).count() * 1e-9 << " s \n\n\n";
    }

    means << EOF;
    variances << EOF;

    means.close();
    variances.close();

    auto ending = chrono::steady_clock::now();
    cout << "Total elapsed time: " << chrono::duration_cast<chrono::nanoseconds>(ending - starting).count() * 1e-9 << endl << endl;

    return 0;
}


double randomGenerator(long digits){
    double e = pow(10, digits);
    return (rand() % static_cast<long>(e)) / e;
}

double mean(vector<long long> data){
    long long sum{0};
    auto size = data.size();
    for (auto object : data)
        sum += object;

    return static_cast<double>(sum) / static_cast<double>(size);
}

double variance(vector<long long> data){
    auto m = mean(data);
    long long res{0};

    for (auto obj : data)
        res += (obj - m) * (obj - m);

    return static_cast<double>(res) / (data.size() - 1);
}

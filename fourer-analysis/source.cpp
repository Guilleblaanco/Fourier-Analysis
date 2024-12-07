#include <fstream>
#include <vector>
#include <complex>
#include <cmath>
#include <sstream>
#include <iostream>

using namespace std;

void readCSV(const string& filename, vector<double>& time, vector<double>& amplitude) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string t, a;

        getline(ss, t, ',');
        getline(ss, a, ',');

        time.push_back(stod(t));
        amplitude.push_back(stod(a));
    }
}

void computeDFT(const vector<double>& signal, vector<complex<double>>& result) {
    if (signal.empty()) {
        cerr << "Error: Signal vector is empty!" << endl;
        return;
    }

    int N = signal.size();
    const double PI = 3.141592653589793;
    result.resize(N);

    for (int k = 0; k < N; ++k) {
        complex<double> sum(0.0, 0.0);
        for (int n = 0; n < N; ++n) {
            double angle = -2.0 * PI * k * n / N;
            sum += polar(signal[n], angle);
        }
        result[k] = sum;
    }
}

void processResults(const vector<complex<double>>& dft, double samplingRate) {
    int N = dft.size();
    for (int k = 0; k < N / 2; ++k) { // Frecuencias positivas
        double magnitude = abs(dft[k]);
        double frequency = k * samplingRate / N;
        cout << "Frequency: " << frequency << " Hz, Magnitude: " << magnitude << '\n';
    }
}

void saveResults(const vector<complex<double>>& dft, double samplingRate, const string& filename) {
    ofstream file(filename);
    int N = dft.size();
    for (int k = 0; k < N / 2; ++k) {
        double magnitude = abs(dft[k]);
        double frequency = k * samplingRate / N;
        file << frequency << "," << magnitude << '\n';
    }
    file.close();
}

int main() {
    vector<double> time, amplitude;
    readCSV("signal.csv", time, amplitude);

    vector<complex<double>> dftResult;
    computeDFT(amplitude, dftResult);

    double samplingRate = 1000.0; // Hz (ejemplo)
    processResults(dftResult, samplingRate);

    saveResults(dftResult, samplingRate, "output.csv");

    return 0;
}

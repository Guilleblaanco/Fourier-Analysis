#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <complex>
#include <cmath>
#include <sstream>
#include <iostream>

using namespace std;
using namespace std::filesystem;

// Reads the file and takes it result to a vector
void readCSV(const string& filename, vector<double>& time, vector<double>& amplitude) {
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cout << "Input file is empty" << endl;
        return;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string t, a;

        getline(ss, t, ',');
        getline(ss, a, ',');

        time.push_back(stod(t));
        amplitude.push_back(stod(a));
    }
}

// Makes the DFT 
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

// Takes the DFT as argument and outputs the result in the console
void processResults(const vector<complex<double>>& dft, double samplingRate) {
    int N = dft.size();
    for (int k = 0; k < N / 2; ++k) { 
        double magnitude = abs(dft[k]); // Magnitude: Intensity of the specified frecuency
        double frequency = k * samplingRate / N;
        cout << "Frequency: " << frequency << " Hz, Magnitude: " << magnitude << '\n';
    }
}

// Generates an output directory
void ensureOutputDirectory(const string& directory) {
    if (!exists(directory)) {
        create_directory(directory);
        cout << "Created directory" << directory << endl;
    }
}

// Generates an unique name for the output file
string generateOutputFilename(const string& inputFilename, const string& directory) {
    string baseName = inputFilename.substr(0, inputFilename.find_last_of('.')); // Default name
    string outputName = directory + "/output_" + baseName + ".csv";

    int counter = 1;
    while (exists(outputName)) { // If the file exists, add a numerical suffix
        outputName = directory + "/output_" + baseName + "_" + to_string(counter) + ".csv";
        ++counter;
    }
    return outputName;
}

// Saves the results in a output.csv file
void saveResults(const vector<complex<double>>& dft, double samplingRate, const string& inputFilename) {
    const string outputDir = "outputs";
    ensureOutputDirectory(outputDir);

    string outputFileName = generateOutputFilename(inputFilename, outputDir);
    ofstream file(outputFileName);
    int N = dft.size();

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for writing.\n";
        return;
    }

    for (int k = 0; k < N / 2; ++k) { // Takes only the meaningful signals (the positive ones)
        double magnitude = abs(dft[k]);
        double frequency = k * samplingRate / N;    
        file << frequency << "," << magnitude << '\n';
    }
    file.close();
}

int main() {
    vector<double> time, amplitude;
    string inputFileName = "signal_TVG.csv"; // Put the name of the input file here
    readCSV(inputFileName, time, amplitude);

    vector<complex<double>> dftResult;
    computeDFT(amplitude, dftResult);

    double samplingRate = 1000.0; // Arbitrary, you may change it depending on the resolution needed
    processResults(dftResult, samplingRate);

    saveResults(dftResult, samplingRate, inputFileName);

    return 0;
}

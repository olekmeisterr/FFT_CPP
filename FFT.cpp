#include <iostream>
#include <cmath>
#include <complex>
using namespace std;
const double PI = 3.14159265359; // Deklaracja pi jako stałej
const int MAX_SIZE = 1024; // Maksymalna ilosc próbek
void FFT(complex<double>* input, complex<double>* output, int step, int size) { // Funkcja fft
    if (size == 1) {
        output[0] = input[0];
        return;
    } // Jezeli jedna próbka, to output to input
    int half = size / 2; // Podział na pół
    FFT(input, output, step * 2, half);         // Część rzeczywista
    FFT(input + step, output + half, step * 2, half); // Część urojona
    for (int k = 0; k < half; k++) {
        complex<double> t = polar(1.0, -2 * PI * k / size) * output[k + half]; // polar generuje liczbę zespoloną o module 1
        output[k + half] = output[k] - t; // Aktualizacja wartości w drugiej połowie danych wyjściowych.
        output[k] += t; // Aktualizacja wartości w pierwszej połowie danych wyjściowych
    }
}
int main() {
    int N; // Inicjalizacja zmiennych
    double Fs; 
    double t = 0.0;
    double Tpr;
    double czestotliwosc[3] = { 0 };
    double amplituda[3] = { 0 };
    double faza[3] = { 0 };
    int ile_skladowych;
    complex<double> signal[MAX_SIZE]; // 
    complex<double> spectrum[MAX_SIZE];
    cout << "Podaj czestotliwosc probkowania [Hz]: ";
    cin >> Fs;
    cout << "Podaj liczbe probek (potega 2): ";
    cin >> N;
    while ((N & (N - 1)) != 0 || N > MAX_SIZE) {
        cout << "Liczba probek musi byc potega dwojki i <= 1024. Podaj ponownie: ";
        cin >> N;
    }
    Tpr = 1 / Fs;
    cout << "----------------------------------------------------------------\n";
    cout << "Podaj ile skladowych bedzie mial sygnal wejsciowy (max 3 skladowe): ";
    cin >> ile_skladowych;
    while (ile_skladowych < 1 || ile_skladowych > 3) {
        cout << "Podales nieprawidlowa liczbe skladowych. Podaj poprawna ilosc skladowych: ";
        cin >> ile_skladowych;
    }
    for (int i = 0; i < ile_skladowych; i++) {
        cout << "Podaj czestotliwosc " << i + 1 << " skladowej w [Hz]: ";
        cin >> czestotliwosc[i];
        cout << "Podaj amplitude " << i + 1 << " skladowej: ";
        cin >> amplituda[i];
        cout << "Podaj przesuniecie fazowe " << i + 1 << " skladowej [rad]: ";
        cin >> faza[i];
    }
    for (int i = 0; i < N; i++) {
        signal[i] = 0; // Inicjalizacja sygnału
        for (int j = 0; j < ile_skladowych; j++) { // Dodawanie do sygnału konkretnych częstotliwości
            signal[i] += amplituda[j] * sin(2 * PI * czestotliwosc[j] * t + faza[j]);
        }
        t += Tpr; // Inkrementacja czasu sygnału o czas próbki
    }
    FFT(signal, spectrum, 1, N); // Zastosowanie funkcji FFT
    cout << "\nWyniki FFT:\n";
    for (int i = 0; i < N; i++) {
        double magnitude = abs(spectrum[i]);
        double phase = arg(spectrum[i]);
        if (abs(spectrum[i]) < 1e-9) {
            abs(spectrum[i]) == 0;
        }
        if (arg(spectrum[i]) < 1e-9) {
            arg(spectrum[i]) == 0;
        }
        if (magnitude < 1e-9) {
            magnitude = 0;
            phase = 0;
        }
        cout << "Czestotliwosc " << i * (Fs / N) << " Hz:\n";
        cout << "  Modul: " << magnitude << "\n";
        cout << "  Faza: " << phase << " rad\n";
        cout << "  Czesc rzeczywista: " << real(spectrum[i]) << "\n";
        cout << "  Czesc urojona: " << imag(spectrum[i]) << "j\n";

    }
    return 0;
}


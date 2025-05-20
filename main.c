#include <stdio.h>
#include <math.h>

// Структура для зберігання вхідних даних кожного варіанту
typedef struct {
    double T;      // Час T (с)
    double dt;     // Крок часу Δt (с)
    double k0;     // Константа k0 (Tn/c)
    double gamma;  // Константа γ
    double e0;     // Заряд e0 (Кл)
    double r;      // Константа r
    double v;      // Швидкість v (м/с)
} Variant;

// Функція для обчислення k(t)
double calculate_k(double t, double T, double k0, double gamma) {
    if (t <= T / 2) {
        return k0 * (1 + gamma);
    } else {
        return k0 * (1 - gamma);
    }
}

// Функція для обчислення B(t)
double calculate_B(double t, double T, double k) {
    if (t <= T / 4) {
        return k * t;
    } else if (t <= 3 * T / 4) {
        return k * T / 4 - k * (t - T / 4);
    } else {
        return 0;
    }
}

// Функція для обчислення e(t)
double calculate_e(double t, double T, double e0, double gamma) {
    if (t <= T / 2) {
        return e0 * (1 + exp(-gamma));
    } else {
        return e0 * (1 + exp(-gamma / 2));
    }
}

// Функція для обчислення сили Лоренца F(t)
double calculate_F(double t, Variant var) {
    double k = calculate_k(t, var.T, var.k0, var.gamma);
    double B = calculate_B(t, var.T, k);
    double e = calculate_e(t, var.T, var.e0, var.gamma);
    double alpha = var.r; // Кут α = r (рад)
    return e * var.v * B * sin(alpha);
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");
    FILE *outputFile = fopen("output.txt", "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Помилка відкриття файлу.\n");
        return 1;
    }

    Variant variants[3]; // Масив структур для трьох варіантів

    // Зчитування вхідних даних з файлу
    for (int i = 0; i < 3; i++) {
        fscanf(inputFile, "%lf %lf %lf %lf %lf %lf %lf",
               &variants[i].T, &variants[i].dt, &variants[i].k0,
               &variants[i].gamma, &variants[i].e0, &variants[i].r,
               &variants[i].v);
    }

    // Обчислення та запис результатів для кожного варіанту
    for (int i = 0; i < 3; i++) {
        fprintf(outputFile, "Варіант %d:\n", i + 1);
        fprintf(outputFile, "Час (t)\tСила (F)\n");

        for (double t = 0; t <= variants[i].T; t += variants[i].dt) {
            double F = calculate_F(t, variants[i]);
            if (F != 0) { // Записувати тільки ненульові значення
                fprintf(outputFile, "%.2f\t%.6e\n", t, F);
            }
        }
        fprintf(outputFile, "\n");
    }

    fclose(inputFile);
    fclose(outputFile);
    printf("Результати записано у файл output.txt\n");

    return 0;
}
#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <algorithm>
#include <memory>
#include <utility>
#include <stdexcept>
#include <cmath>

class Vector3D {
private:
    static constexpr size_t DIM = 3;          // трёхмерный вектор: ровно 3 координаты
    std::unique_ptr<double[]> coords;         // умный указатель на динамический массив

public:
    // 1. Конструктор
    explicit Vector3D(double x = 0.0, double y = 0.0, double z = 0.0)
        : coords(std::make_unique<double[]>(DIM)) {
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
        std::cout << "Vector created (" << x << ", " << y << ", " << z << ")\n";
    }

    // 2. Деструктор
    ~Vector3D() {
        std::cout << "Vector destroyed\n";
    }

    // 3. Конструктор копирования (глубокое копирование)
    //std::copy(начало, конец, куда_писать) — алгоритм из <algorithm>
    Vector3D(const Vector3D& other)
        : coords(std::make_unique<double[]>(DIM)) {
        std::copy(other.coords.get(), other.coords.get() + DIM, coords.get());
        std::cout << "Vector created (copy)\n";
    }

    // 4. Оператор присваивания копированием
    Vector3D& operator=(const Vector3D& other) {
        if (this != &other) {
            auto tmp = std::make_unique<double[]>(DIM);
            std::copy(other.coords.get(), other.coords.get() + DIM, tmp.get());
            coords = std::move(tmp);
        }
        std::cout << "Copy assignment completed\n";
        return *this;
    }

    // 5. Конструктор перемещения
    //Vector3D&& — rvalue‑ссылка
    Vector3D(Vector3D&& other) noexcept
        : coords(std::move(other.coords)) {
        std::cout << "Move constructor\n";
    }

    // 6. Оператор присваивания перемещением
    Vector3D& operator=(Vector3D&& other) noexcept {
        if (this != &other) {
            coords = std::move(other.coords);
        }
        std::cout << "Move assignment\n";
        return *this;
    }

    // Доступ к координатам с проверкой границ
    double& operator[](size_t i) {
        if (i >= DIM) throw std::out_of_range("Index out of range");
        return coords[i];
    }

    const double& operator[](size_t i) const {
        if (i >= DIM) throw std::out_of_range("Index out of range");
        return coords[i];
    }

    void print(const char* name) const {
        std::cout << name << " = (" << coords[0] << ", "
                  << coords[1] << ", " << coords[2] << ")\n";
    }
};

// Функция принимает std::shared_ptr<Vector3D> и нормализует вектор
//(*v)[i] — вызываем перегруженный оператор [], через него получаем доступ к координатам coords[i];
void normalize(const std::shared_ptr<Vector3D>& v) {
    const double x = (*v)[0];
    const double y = (*v)[1];
    const double z = (*v)[2];

    const double length = std::sqrt(x * x + y * y + z * z);

    if (length == 0.0) {
        throw std::invalid_argument("Cannot normalize a zero vector");
    }

    (*v)[0] = x / length;
    (*v)[1] = y / length;
    (*v)[2] = z / length;

    std::cout << "Normalization done, length = " << length << "\n";
    v->print("normalized vector");
}

#endif // VECTOR3D_H
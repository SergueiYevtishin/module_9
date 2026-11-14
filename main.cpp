#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <utility>

#include "vector3d.h"

// ---------- Move tests ----------

void test_move_constructor() {
    std::cout << "=== Test 1: move constructor ===\n";

    Vector3D source(1.0, 2.0, 3.0);
    Vector3D moved = std::move(source);   // "Move constructor"

    assert(moved[0] == 1.0 && moved[1] == 2.0 && moved[2] == 3.0);

    moved.print("moved");

    std::cout<< "\nasset transferred\n";
}

void test_move_assignment() {
    std::cout << "=== Test 2: move assignment operator ===\n";

    Vector3D target(7.0, 7.0, 7.0);
    Vector3D source(4.0, 5.0, 6.0);

    target = std::move(source);           // "Move assignment"
    target.print("target");

    assert(target[0] == 4.0 && target[1] == 5.0 && target[2] == 6.0);

    // Самоприсваивание через алиас (без предупреждения компилятора)
    Vector3D self(1.0, 1.0, 1.0);
    Vector3D& ref = self;
    self = std::move(ref);
    self.print("self after self-move");

    std::cout << "\n";
}

void test_move_vs_copy() {
    std::cout << "=== Test 3: move vs copy ===\n";

    Vector3D a(1.0, 2.0, 3.0);
    Vector3D copied = a;                  // "Vector created (copy)"
    Vector3D moved = std::move(a);        // "Move constructor"

    std::cout << "\n";
}

// ---------- unique_ptr tests ----------

void test_unique_ptr_lifetime() {
    std::cout << "=== Test 4: unique_ptr lifetime ===\n";

    {
        auto p1 = std::make_unique<Vector3D>(1.0, 1.0, 1.0);

        auto p2 = std::move(p1);
        assert(p1 == nullptr);
        assert(p2 != nullptr);
        p2->print("p2");

        p2 = std::make_unique<Vector3D>(5.0, 5.0, 5.0);
        // здесь: "Vector destroyed" для старого объекта
    }
    // здесь: "Vector destroyed" для второго объекта

    std::cout << "unique_ptr freed the memory automatically\n\n";
}


// ---------- shared_ptr tests ----------

void test_shared_ptr_lifetime() {
    std::cout << "=== Test 5: shared_ptr reference counting ===\n";

    auto shared1 = std::make_shared<Vector3D>(3.0, 4.0, 12.0);
    std::cout << "use_count after creation: " << shared1.use_count() << "\n";

    {
        auto shared2 = shared1;
        std::cout << "use_count inside block: " << shared1.use_count() << "\n";
        assert(shared1.get() == shared2.get());

        normalize(shared2);
        shared2->print("shared2");
    }
    // Объект жив — им владеет shared1

    std::cout << "use_count after block: " << shared1.use_count()
              << " — object was not destroyed prematurely\n";

    normalize(shared1);

    std::cout << "\n";
}


void test_weak_ptr() {
    std::cout << "=== Test 6: weak_ptr does not keep object alive ===\n";

    std::weak_ptr<Vector3D> observer;
    {
        auto shared = std::make_shared<Vector3D>(1.0, 1.0, 1.0);
        observer = shared;
        std::cout << "object alive inside block: "
                  << (observer.lock() != nullptr) << "\n";
    }
    // здесь: "Vector destroyed"

    std::cout << "object alive after block: "
              << (observer.lock() != nullptr) << "\n\n";
}

// ---------- main ----------

int main() {
    test_move_constructor();
    test_move_assignment();
    test_move_vs_copy();

    test_unique_ptr_lifetime();
    test_shared_ptr_lifetime();
    test_weak_ptr();

    std::cout << "All tests passed\n";
    return 0;
}
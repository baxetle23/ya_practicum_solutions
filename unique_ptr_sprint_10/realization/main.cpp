#include <cassert>
#include <cstddef> 

using namespace std;

template <typename T>
class UniquePtr {
private:
    T* value = nullptr;
public:
    UniquePtr() {
        value = nullptr;
    }
    explicit UniquePtr(T* ptr) {
        value = ptr;
    }
    UniquePtr(const UniquePtr&)  = delete;
    UniquePtr(UniquePtr&& other) : value(other.Release()) {}
    UniquePtr& operator=(const UniquePtr&) = delete;
    UniquePtr& operator=(nullptr_t) {
        delete value;
        value = nullptr;
    }
    UniquePtr& operator=(UniquePtr&& other) {
        if (this->Get() == other.Get())
            return this;
        *value = std::move(*other);
        return value;
    }
    ~UniquePtr() {
        if (value != nullptr)
            delete value;
    }

    T& operator*() const {
        return *value;
    }
    T* operator->() const {
        return value;
    }
    T* Release() {
        auto tmp = value;
        value = nullptr;
        return tmp;
    }
    void Reset(T* ptr) {
        delete value;
        value = ptr;
    }
    void Swap(UniquePtr& other) {
        T* tmp;
        tmp = other;
        other.Reset(value);
        this->Reset(tmp);
    }
    T* Get() const {
        return value;
    }
};

struct Item {
    static int counter;
    int value;
    Item(int v = 0)
        : value(v) 
    {
        ++counter;
    }
    Item(const Item& other)
        : value(other.value) 
    {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;

void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        ptr.Reset(new Item);
        assert(Item::counter == 1);
    }
    assert(Item::counter == 0);

    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        auto rawPtr = ptr.Release();
        assert(Item::counter == 1);

        delete rawPtr;
        assert(Item::counter == 0);
    }
    assert(Item::counter == 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    assert(ptr.Get()->value == 42);
    assert((*ptr).value == 42);
    assert(ptr->value == 42);
}

int main() {
    TestLifetime();
    TestGetters();
}

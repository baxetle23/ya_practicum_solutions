#include <cassert>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

struct Cat {
    Cat(const string& name, int age)
        : name_(name)
        , age_(age)  //
    {
    }
    const string& GetName() const noexcept {
        return name_;
    }
    int GetAge() const noexcept {
        return age_;
    }
    ~Cat() {
    }
    void Speak() const {
        cout << "Meow!"s << endl;
    }

private:
    string name_;
    int age_;
};

// ??????? ??????? ?????????? ?????
unique_ptr<Cat> CreateCat(const string& name) {
    return make_unique<Cat>(name, 2);
}

class Witch {
public:
    explicit Witch(const string& name)
        : name_(name) {
    }
    
    explicit Witch(const Witch& other_witch) {
        name_ = other_witch.GetName();
        cat_ = make_unique<Cat>(other_witch.cat_->GetName(), other_witch.cat_->GetAge());
    }
    
    explicit Witch(Witch&& witch) = default;
    
    Witch& operator=(Witch& other_witch) {
        Witch tmp(other_witch);
        this->SetCat(tmp.ReleaseCat());
        this->name_ = tmp.GetName();
        return *this;
    }
    
    Witch& operator=(Witch&& witch) = default;

    const string& GetName() const noexcept {
        return name_;
    }
    void SetCat(unique_ptr<Cat>&& cat) noexcept {
        cat_ = std::move(cat);
    }
    unique_ptr<Cat> ReleaseCat() noexcept {
        return std::move(cat_);
    }

private:
    string name_;
    unique_ptr<Cat> cat_;
};

void Test() {
    // ??????? Witch ????? ??????????
    {
        Witch witch("Hermione"s);
        auto cat = CreateCat("Crookshanks"s);
        Cat* raw_cat = cat.get();
        assert(raw_cat);
        witch.SetCat(move(cat));

        Witch moved_witch(std::move(witch));
        auto released_cat = moved_witch.ReleaseCat();
        assert(released_cat.get() == raw_cat);  // ??? ???????????? ?? witch ? moved_witch
    }

    // ????? ???????????? ???????????? ???????? ????????????
    {
        Witch witch("Hermione"s);
        auto cat = CreateCat("Crookshanks"s);
        Cat* raw_cat = cat.get();
        witch.SetCat(move(cat));

        Witch witch2("Minerva McGonagall");
        witch2 = move(witch);
        auto released_cat = witch.ReleaseCat();
        assert(!released_cat);
        released_cat = witch2.ReleaseCat();
        assert(released_cat.get() == raw_cat);
    }

    // ????? ?????????? ?????????
    {
        Witch witch("Hermione");
        auto cat = CreateCat("Crookshanks"s);
        witch.SetCat(move(cat));

        Witch witch_copy(witch);
        assert(!cat);
        cat = witch.ReleaseCat();
        assert(cat);  // ? ?????? ?????????? ??? ?????? ?? ?????

        auto cat_copy = witch_copy.ReleaseCat();
        assert(cat_copy != nullptr && cat_copy != cat);
        assert(cat_copy->GetName() == cat->GetName());  // ????? ?????????? ???????? ????? ????
    }

    // ???????? ?????????? ???????????? ?????????
    {
        Witch witch("Hermione"s);
        auto cat = CreateCat("Crookshanks"s);
        witch.SetCat(move(cat));

        Witch witch2("Minerva McGonagall"s);
        witch2 = witch;

        assert(!cat);
        cat = witch.ReleaseCat();
        assert(cat);  // ? ?????? ?????????? ??? ?????? ?? ?????

        auto cat_copy = witch2.ReleaseCat();
        assert(cat_copy != nullptr && cat_copy != cat);
        assert(cat_copy->GetName() == cat->GetName());  // ??? ???????????? ???????????? ???
    }
}

int main() {
    Test();
}
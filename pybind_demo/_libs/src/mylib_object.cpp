#include <iostream>
#include <pybind11/pybind11.h>

namespace py = pybind11;

// Exp 1: raw inheritance 
struct Pet {
    Pet(const std::string &name): name(name) {} // initialization list
    void setName(const std::string &name_) {name = name_;}
    const std::string &getName() const {return name;}

    std::string name;
};

struct Dog : Pet {
    Dog(const std::string &name): Pet(name) {}
    std::string bark() const {return "woof!";}
};

// Exp 2: Polymophic inheritance 
struct PolymorphicPet {
    virtual ~PolymorphicPet() = default;
};

struct PolymorphicDog : PolymorphicPet {
    std::string bark() const {return "woof!";}
};

// Exp 3: overload
struct PetOL {
    PetOL(const std::string &name, int age) : name(name), age(age) { }

    void set(int age_) { age = age_; }
    void set(const std::string &name_) { name = name_; }

    std::string name;
    int age;
};

// Exp 4: enumerations and internal types
struct Pet_EI {
    enum Kind {
        Dog = 0,
        Cat
    };

    struct Attributes {
        float age = 0;
    };

    Pet_EI(const std::string &name, Kind type) : name(name), type(type) {}

    std::string name;
    Kind type;
    Attributes attr;
};

PYBIND11_MODULE(mylib_object, m) {
    // Exp 1:
    // "class_" creates bindings for a C++ class or struct-style data structure.
    // "init" is a func that takes the "types of a constructor's parameters" as 
    //   template arguments and wraps the corresponding constructor.
    // use empty [] as the capture object means it's a stateless closure.
    // "py::dynamic_attr()" enable adding new(unseen) attributes to python obj. 
    //   it's Optional and comes with a small overhead
    py::class_<Pet>(m, "Pet", py::dynamic_attr())
        .def(py::init<const std::string &>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName)
        .def("__repr__", [](const Pet &a) {
            return "<example.Pet named '" + a.name + "'>";
        })
        .def_readwrite("name", &Pet::name); // can directly operate 'name', replace set/getName

    m.def("pet_store", []() {return std::unique_ptr<Pet>(new Dog("Molly"));});


    // Exp 2:
    py::class_<Dog, Pet>(m, "Dog") // represents Dog inherits Pet
        .def(py::init<const std::string &>())
        .def("bark", &Dog::bark);

    py::class_<PolymorphicPet>(m, "PolymorphicPet");
    py::class_<PolymorphicDog, PolymorphicPet>(m, "PolymorphicDog")
        .def(py::init<>())
        .def("bark", &PolymorphicDog::bark);

    m.def("pet_store2", []() {return std::unique_ptr<PolymorphicPet>(new PolymorphicDog);});


    // Exp 3:
    // Disambiguate them by casting them to function pointers. Binding multiple functions to the same Python name automatically 
    // creates a chain of function overloads that will be tried in sequence.
    py::class_<PetOL>(m, "PetOL")
        .def(py::init<const std::string &, int>())
        .def("set", static_cast<void (PetOL::*)(int)>(&PetOL::set), "Set the pet's age")
        .def("set", static_cast<void (PetOL::*)(const std::string &)>(&PetOL::set), "Set the pet's name")
        .def_readwrite("name", &PetOL::name)
        .def_readwrite("age", &PetOL::age);


    // Exp 4:
    py::class_<Pet_EI> pet(m, "Pet_EI");
    // init func
    pet.def(py::init<const std::string &, Pet_EI::Kind>())
        .def_readwrite("name", &Pet_EI::name)
        .def_readwrite("type", &Pet_EI::type)
        .def_readwrite("attr", &Pet_EI::attr);
    py::enum_<Pet_EI::Kind>(pet, "Kind")
        .value("Dog", Pet_EI::Kind::Dog)
        .value("Cat", Pet_EI::Kind::Cat)
        .export_values(); // exports enum entries into the parent scope

    py::class_<Pet_EI::Attributes>(pet, "Attributes")
        .def(py::init<>())
        .def_readwrite("age", &Pet_EI::Attributes::age);
}

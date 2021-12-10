#include <iostream>
#include <memory>

class Toy
{
    std::string name;
public:

    Toy(std::string name)
    {
        this->name = name;
        std::cout << "Toy name - " << this->name << std::endl;
    }

    Toy(): name("Unknown") {}

    std::string getName()
    {
        return name;
    }
};


class Shared_ptr_toy
{
    Toy* toy = nullptr; //указатель
    int* count = nullptr;

public:

    //конструктор по умолчанию
    Shared_ptr_toy()
    {
        toy = new Toy();
        count = new int(1);
        std::cout << "Default constructor " << this << std::endl;
    }

    //конструктор с аргументом name
    Shared_ptr_toy(std::string name)
    {
        toy = new Toy(name);
        count = new int(1);
        std::cout << "Constructor - " << this << ", count = " << *count << std::endl;
    }

    //конструктор копирования
    Shared_ptr_toy(const Shared_ptr_toy& other)
    {
        toy = other.toy;
        count = other.count;
        ++*count;
        std::cout << "Constructor " << this << ", count = " << *count << ", name " << toy->getName() << std::endl;
    }

    //перегрузка оператора присваивания
    Shared_ptr_toy& operator= (const Shared_ptr_toy& other)
    {
        if (this == &other) return *this;

        //если указатель не нулевой...
        if (toy != nullptr)
        {

            --*count; //счетчик уменьшаем

            //если нет ни одной ссылки, указывающей на данный объект
            if (*count == 0)
            {
                delete toy; //очищаем память объекта
                delete count; //очищаем память счетчика
                toy = nullptr; //обнуляем указатель на объект
                count = nullptr; //обнуляем указатель счетчика
            }
        }

        toy = other.toy; //присваиваем данные из другого объекта, включая выделение новой памяти
        count = other.count;
        ++*count;

        return *this;
    }

    //деструктор
    ~Shared_ptr_toy()
    {
        if (toy != nullptr)
        {
            --*count; //счетчик уменьшаем
            if (*count == 0)
            {
                delete toy;
                delete count;
                toy = nullptr;
                count = nullptr;
            }
        }

        if (count != nullptr)
            std::cout << std::endl << "Destructor " << this << " count = " << *count;
        else
            std::cout << std::endl << "Toy deleted" << std::endl;
    }
};


  /*template<typename ... Args>
  Shared_ptr_toy make_shared_toy(Args&&... args)
  {
    return  Shared_ptr_toy(std::forward<Args>(args)...);
  }*/


   template <typename T>
   Shared_ptr_toy make_shared_toy (T value)
   {
       //если value - строка, то создается игрушка от имени
       //если value - объект, то создается копия этого объекта
       Shared_ptr_toy a(value);
       return a; //возвращает объект
   }

int main() {

    Shared_ptr_toy a = make_shared_toy<std::string>("Sock"); //конструируем игрушку от имени

    Shared_ptr_toy b = make_shared_toy<Shared_ptr_toy>(a); //создаем копию игрушки от переданной в качестве аргумента другой игрушки
}
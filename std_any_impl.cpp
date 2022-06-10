/* implement std::any in c++ */
#include <iostream>
#include <memory>
#include <typeinfo>
#include <utility>

struct ObjectBase
{
	// all the interfaces required to know the information of the object
	virtual const std::type_info& getType() const = 0;
	virtual std::unique_ptr<ObjectBase> clone() const = 0;
	virtual ~ObjectBase() {}
};

template<typename T>
struct Wrapper : public ObjectBase
{
public:
	Wrapper(const T& obj) : wrappedObject_(obj) { std::cout << "copy ctor\n"; }
	Wrapper(T&& obj) : wrappedObject_(std::move(obj)) { std::cout << "move ctor\n"; }

	std::unique_ptr<ObjectBase> clone() const override
	{
		auto ptr = new Wrapper<T>(this->wrappedObject_);
		// auto ptr1{std::make_unique<ObjectBase>(ptr)};
		return std::unique_ptr<ObjectBase>(ptr);
	}

	const std::type_info& getType() const override
	{
		return typeid(T);
	}

	T& getObject()
	{
		return wrappedObject_;
	}

private:
	T wrappedObject_;
};

class bad_cast
{
public:
	virtual const char* what() = 0;
};

class bad_any_cast : public bad_cast
{
public:
	const char* what() override
	{
		return "Bad cast exception";
	}
};

class Any
{
private:	
	std::unique_ptr<ObjectBase> wrappedObject_;

public:
	// default constructor
	Any() = default;
	
	// constructor to create any type object
	template<typename T>
	Any(const T& obj)
	{
		wrappedObject_ = std::make_unique<Wrapper<T>>(std::move(obj));
	}

	// copy constructor
	Any(const Any& obj)
	{
		std::cout << "calling copy ctor" << std::endl;
		wrappedObject_ = obj.wrappedObject_->clone();
	}

	// operator=
	Any& operator=(Any obj)
	{
		std::cout << "calling assignment operator" << std::endl;
		std::swap(obj.wrappedObject_, this->wrappedObject_);
		return *this;
	}

	// type method to return the name of the type currently holding
	const std::type_info& type() const
	{
		return wrappedObject_->getType();
	}

	// destroy the current object held
	void reset()
	{
		wrappedObject_.reset();
	}

	// check if the object holds any value
	bool has_value() const
	{
		return static_cast<bool>(wrappedObject_);
	}

	template<typename T>
	friend T& any_cast(const Any& obj)
	{
		// do type checking to find if the object and the required type match
		if(obj.type().hash_code() != typeid(T).hash_code())
		{
			throw bad_any_cast();
		}
		// get the wrapper type from the base object type by downcasting
		auto ptr = dynamic_cast<Wrapper<T>*>(obj.wrappedObject_.get());
		if(ptr == nullptr)
		{
			throw bad_any_cast();
		}
		return ptr->getObject();
		// return *static_cast<T*>(obj.wrappedObject_.get());
	}
};

int main(int argc, char const *argv[])
{
	Any a;
	try
	{
		a = 1;
		if(a.has_value())
		{
			std::cout << any_cast<int>(a) << std::endl;
		}
		a = 3.14;
		std::cout << any_cast<double>(a) << std::endl;
		Any b = 70.5;
		std::cout << any_cast<double>(b) << std::endl;
	}
	catch(bad_any_cast& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
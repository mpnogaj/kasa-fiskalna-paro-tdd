#pragma once

#include <string>

class Product
{
  protected:
	std::string _name;
	double _price;

	Product(const std::string &name, double price);

  public:
	virtual ~Product() = default;

	inline virtual std::string getName() const
	{
		return _name;
	}
	inline virtual double getPrice() const
	{
		return _price;
	}
};

class ProductByWeight : public Product
{
  public:
	ProductByWeight(const std::string &name, double pricePerKg) : Product(name, pricePerKg)
	{
	}
};

class ProductByPiece : public Product
{
  public:
	ProductByPiece(const std::string &name, double pricePerUnit) : Product(name, pricePerUnit)
	{
	}
};

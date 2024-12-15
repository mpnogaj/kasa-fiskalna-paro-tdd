#pragma once

#include <memory>
#include <string>

#include "product.hpp"

class ReceiptItem
{
  public:
	virtual ~ReceiptItem() = default;
	virtual double calculatePrice() const = 0;
	virtual std::string getName() const = 0;
	virtual std::string getDetails() const = 0;
};

class ReceiptItemByWeight : public ReceiptItem
{
  private:
	const std::shared_ptr<ProductByWeight> product;
	const double weight;

  public:
	ReceiptItemByWeight(const std::shared_ptr<ProductByWeight> product, double weight);

	inline double calculatePrice() const override
	{
		return product->getPrice() * weight;
	}

	inline std::string getName() const override
	{
		return product->getName();
	}

	inline std::string getDetails() const override
	{
		return product->getName() + ": " + std::to_string(weight) + " kg x " + std::to_string(product->getPrice()) +
			   " = " + std::to_string(calculatePrice());
	}
};

class ReceiptItemByPiece : public ReceiptItem
{
  private:
	const std::shared_ptr<ProductByPiece> product;
	const int quantity;

  public:
	ReceiptItemByPiece(const std::shared_ptr<ProductByPiece> product, int quantity);

	inline double calculatePrice() const override
	{
		return product->getPrice() * quantity;
	}

	inline std::string getName() const override
	{
		return product->getName();
	}

	inline std::string getDetails() const override
	{
		return product->getName() + ": " + std::to_string(quantity) + " pcs x " + std::to_string(product->getPrice()) +
			   " = " + std::to_string(calculatePrice());
	}
};

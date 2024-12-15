#pragma once

#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

#include "product.hpp"
#include "receipt-item.hpp"
#include "receipt.hpp"

class Register
{
  private:
	std::unordered_map<std::string, std::shared_ptr<Product>> productCatalog;
	Receipt activeReceipt;

  public:
	bool addProductToCatalog(const std::shared_ptr<Product> product);
	bool removeProductFromCatalog(const std::string &name);

	bool addToReceipt(const std::string &name, std::variant<int, double> quantity);
	bool removeFromReceipt(const std::string &name);
	double getActiveReceiptValue() const;

	std::vector<std::string> getReceiptDetails() const;
	std::vector<std::shared_ptr<const Product>> getProductCatalog() const;
};

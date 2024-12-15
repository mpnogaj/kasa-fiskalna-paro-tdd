#pragma once

#include <memory>
#include <string>
#include <vector>

#include "receipt-item.hpp"

class Receipt
{
  private:
	std::vector<std::unique_ptr<ReceiptItem>> items;

  public:
	bool addItem(std::unique_ptr<ReceiptItem> receiptItem);
	bool addItem(const std::shared_ptr<ProductByPiece> product, int count);
	bool addItem(const std::shared_ptr<ProductByWeight> product, double weight);

	bool removeItem(const std::string &name);

	double getTotalValue() const;

	std::vector<std::string> getDetails() const;
	inline int itemCount() const
	{
		return items.size();
	}
};

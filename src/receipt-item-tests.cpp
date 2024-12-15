#include <gtest/gtest.h>

#include <string>

#include "receipt-item.hpp"

TEST(ReceiptItemByWeightTest, CalculatesCorrectPrice)
{
	const std::string name = "Apples";
	const double price = 3.5;
	const double weight = 2.5;
	const double expected_price = price * weight;
	const std::string expected_details = name + ": " + std::to_string(weight) + " kg x " + std::to_string(price) +
										 " = " + std::to_string(expected_price);

	auto product = std::make_shared<ProductByWeight>(name, price);
	ReceiptItemByWeight item(product, weight);

	EXPECT_DOUBLE_EQ(item.calculatePrice(), price * weight);
	EXPECT_EQ(item.getName(), name);
	EXPECT_EQ(item.getDetails(), expected_details);
}

TEST(ReceiptItemByWeightTest, ProductCannotBeNull)
{
	auto product = std::shared_ptr<ProductByWeight>(nullptr);

	ASSERT_THROW(ReceiptItemByWeight(product, 12.2), std::invalid_argument);
}

TEST(ReceiptItemByWeightTest, WeightCannotBeZeroOrNegative)
{
	auto product = std::make_shared<ProductByWeight>("Apples", 3.5);

	ASSERT_THROW(ReceiptItemByWeight(product, -12.2), std::invalid_argument);
}

TEST(ReceiptItemByPieceTest, CalculatesCorrectPrice)
{
	double price = 1.2;
	int pcs = 5;
	double expected_price = price * pcs;
	std::string name = "Bananas";
	const std::string expected_details =
		name + ": " + std::to_string(pcs) + " pcs x " + std::to_string(price) + " = " + std::to_string(expected_price);

	std::shared_ptr<ProductByPiece> product = std::make_shared<ProductByPiece>("Bananas", price);
	ReceiptItemByPiece item(product, 5);

	EXPECT_DOUBLE_EQ(item.calculatePrice(), expected_price);
	EXPECT_EQ(item.getName(), name);
	EXPECT_EQ(item.getDetails(), expected_details);
}

TEST(ReceiptItemByPieceTest, ProductCannotBeNull)
{
	auto product = std::shared_ptr<ProductByPiece>(nullptr);
	ASSERT_THROW(ReceiptItemByPiece(product, 12.2), std::invalid_argument);
}

TEST(ReceiptItemByPieceTest, WeightCannotBeZeroOrNegative)
{
	auto product = std::make_shared<ProductByPiece>("Apples", 3.5);
	ASSERT_THROW(ReceiptItemByPiece(product, -10), std::invalid_argument);
}

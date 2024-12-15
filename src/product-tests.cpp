#include <gtest/gtest.h>

#include "product.hpp"

TEST(ProductByWeightTest, ReturnsCorrectNameAndPrice)
{
	ProductByWeight product("Apples", 3.5);
	EXPECT_EQ(product.getName(), "Apples");
	EXPECT_DOUBLE_EQ(product.getPrice(), 3.5);
}

TEST(ProductByWeightTest, NameCannotBeEmpty)
{
	EXPECT_THROW(ProductByWeight("", 1.2), std::invalid_argument);
}

TEST(ProductByWeightTest, PriceCannotBeNegative)
{
	EXPECT_THROW(ProductByWeight("Apples", -1.2), std::invalid_argument);
}

TEST(ProductByPieceTest, ReturnsCorrectNameAndPrice)
{
	ProductByPiece product("Bananas", 1.2);
	EXPECT_EQ(product.getName(), "Bananas");
	EXPECT_DOUBLE_EQ(product.getPrice(), 1.2);
}

TEST(ProductByPieceTest, NameCannotBeEmpty)
{
	EXPECT_THROW(ProductByPiece("", 1.2), std::invalid_argument);
}

TEST(ProductByPieceTest, PriceCannotBeNegative)
{
	EXPECT_THROW(ProductByPiece("Apples", -1.2), std::invalid_argument);
}

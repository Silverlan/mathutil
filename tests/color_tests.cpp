#include <memory>
#include "mathutil/color.h"
#include "gtest/gtest.h"
#include "gtest_common.h"

TEST(ColorTests, CreateFromHexColor_RGB) {
	Color color1 = Color::CreateFromHexColor("f0bc18");
	
	ASSERT_EQ(color1.r, 240);
	ASSERT_EQ(color1.g, 188);
	ASSERT_EQ(color1.b, 24);
	ASSERT_EQ(color1.a, 255);
}
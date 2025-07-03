#include "gmock/gmock.h"
#include "device_driver.h"
#include "custom_exception.h"

class FlashMemoryDeviceMock : public FlashMemoryDevice {
public:
	//virtual unsigned char read(long address) = 0;
	//virtual void write(long address, unsigned char data) = 0;

	MOCK_METHOD(unsigned char, read, (long), (override));
	MOCK_METHOD(void, write, (long, unsigned char), (override));
};

// 나중에 fixture도 도입 예정

TEST(DeviceDriver, ReadFromHWWithCorrectAnswer) {
	FlashMemoryDeviceMock hw;
	DeviceDriver dd{ &hw };

	// read를 5번 수행해야해
	EXPECT_CALL(hw, read)
		.Times(5)
		.WillRepeatedly(testing::Return(0x5A));


	int data = dd.read(0xFF);
}

TEST(DeviceDriver, ReadFromHWWithErrorAnswer) {
	// read 마지막한개는 다른 거면 exception 발생해야해.
	FlashMemoryDeviceMock hw;
	DeviceDriver dd{ &hw };

	// read를 5번 수행해야해
	EXPECT_CALL(hw, read)
		.Times(5)
		.WillOnce(testing::Return(0x5A))
		.WillOnce(testing::Return(0x5A))
		.WillOnce(testing::Return(0x5A))
		.WillOnce(testing::Return(0x5A))
		.WillOnce(testing::Return(0x5E));

	//EXPECT_THROW(dd.read(0xFF), std::ReadFailExcetion);
	EXPECT_THROW(dd.read(0xFF), std::exception);
	
}

//TEST(DeviceDriver, ReadFromHW) {
//	// TODO : replace hardware with a Test Double
//	//FlashMemoryDevice* hardware = nullptr;
//	FlashMemoryDevice* hw = new FlashMemoryDeviceMock();
//	DeviceDriver driver{ &hw };
//
//	EXPECT_CALL(hw, read)
//		.Times(5)
//		.WillRepeatedly(testing::Return(0x5A));
//
//
//	int data = driver.read(0xFF);
//	EXPECT_EQ(0x5A, data);
//}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
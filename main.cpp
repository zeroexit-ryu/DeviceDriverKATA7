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

// ���߿� fixture�� ���� ����

TEST(DeviceDriver, ReadFromHWWithCorrectAnswer) {
	FlashMemoryDeviceMock hw;
	DeviceDriver dd{ &hw };

	// read�� 5�� �����ؾ���
	EXPECT_CALL(hw, read)
		.Times(5)
		.WillRepeatedly(testing::Return(0x5A));


	int data = dd.read(0xFF);
}

TEST(DeviceDriver, ReadFromHWWithErrorAnswer) {
	// read �������Ѱ��� �ٸ� �Ÿ� exception �߻��ؾ���.
	FlashMemoryDeviceMock hw;
	DeviceDriver dd{ &hw };

	// read�� 5�� �����ؾ���
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
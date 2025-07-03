#include "gmock/gmock.h"
#include "device_driver.h"
#include "custom_exception.h"
#include "Application.cpp"

using namespace testing;

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

TEST(DeviceDriver, WriteEmptyAddress) {
	// Wrtie ������ 0xFF�� �Ǿ� �ִٸ� Write�� �ȴ�.
	NiceMock<FlashMemoryDeviceMock> hw;
	DeviceDriver dd{ &hw };
	
	EXPECT_CALL(hw, read(0xF))
		.Times(1)
		.WillOnce(testing::Return(0xFF));

	dd.write((long)0xF, 0x5A);
}

TEST(DeviceDriver, WriteNotEmptyAddress) {
	// Wrtie ������ 0xFF�� �ƴ� ���� �ִٸ� Exception �߻�
	FlashMemoryDeviceMock hw;
	DeviceDriver dd{ &hw };

	EXPECT_CALL(hw, read(0xF))
		.Times(1)
		.WillOnce(testing::Return(0xFA));

	EXPECT_THROW(dd.write(0xF, 0x5A), std::exception);
}

TEST(Application, ReadAndPrintBehaviorTest) {
	FlashMemoryDeviceMock hw;
	DeviceDriver dd{ &hw };
	Application app{ &dd };

	EXPECT_CALL(hw, read(_))
		.Times(25)
		.WillRepeatedly((testing::Return(0xFA)));

	app.readAndPrint(0x0, 0x4);
}

TEST(Application, WriteAllTest) {
	NiceMock<FlashMemoryDeviceMock> hw;
	DeviceDriver dd{ &hw };
	Application app{ &dd };

	EXPECT_CALL(hw, read(_))
		.Times(5)
		.WillRepeatedly((testing::Return(0xFF)));

	app.writeAll(0xA2);
}

//TEST(Application, ReadAndPrintTest) {
//	FlashMemoryDeviceMock hw;
//	DeviceDriver dd{ &hw };
//	Application app{ &dd };
//
//	EXPECT_CALL(hw, read(0x0))
//		.WillRepeatedly((testing::Return(9)));
//	EXPECT_CALL(hw, read(0x1))
//		.WillRepeatedly((testing::Return(8)));
//	EXPECT_CALL(hw, read(0x2))
//		.WillRepeatedly((testing::Return(7)));
//	EXPECT_CALL(hw, read(0x3))
//		.WillRepeatedly((testing::Return(6)));
//	EXPECT_CALL(hw, read(0x4))
//		.WillRepeatedly((testing::Return(5)));
//	
//	app.readAndPrint(0, 5);
//
//	//EXPECT_EQ("98765", app.readAndPrint(0, 5));
//
//	//app.writeAll();
//}

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
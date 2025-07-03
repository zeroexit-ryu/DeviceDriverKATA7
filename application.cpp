#include <iostream>
#include "device_driver.h"

using std::cout;
using std::endl;

class Application {
public:
	Application(DeviceDriver* dd) : dd{ dd } {}
	void readAndPrint(int startAddr, int endArr) {
		for (int addr = startAddr; addr <= endArr; ++addr)
			cout << dd->read(addr) << endl;
	}

	void writeAll(int value) {
		for (int addr = 0x00; addr <= 0x04; ++addr) {
			dd->write(addr, value);
		}
	}

private:
	DeviceDriver* dd;
};


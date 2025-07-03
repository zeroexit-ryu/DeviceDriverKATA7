#include "device_driver.h"
#include "custom_exception.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}
const int TestTryCount = 4;

int DeviceDriver::read(long address)
{
    int ret = (int)(m_hardware->read(address));

    for (int turn = 0; turn < TestTryCount; turn++) {
        int testvalue = (int)(m_hardware->read(address));
        if (testvalue != ret) throw ReadFailException();
    }
    return (int)(ret);
}

void DeviceDriver::write(long address, int data)
{
    int old_data = m_hardware->read(address);
    if (old_data != 0xFF) throw WriteFailException();

    m_hardware->write(address, (unsigned char)data);
}
// -*- C++ -*-
/*!
 * @file  Dummy_Gamepad.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "Dummy_Gamepad.h"

// Module specification
// <rtc-template block="module_spec">
static const char* dummy_gamepad_spec[] =
  {
    "implementation_id", "Dummy_Gamepad",
    "type_name",         "Dummy_Gamepad",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "Yamamoto",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Dummy_Gamepad::Dummy_Gamepad(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_JoystickOut("Joystick", m_Joystick),
    m_ButtonsOut("Buttons", m_Buttons)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Dummy_Gamepad::~Dummy_Gamepad()
{
}



RTC::ReturnCode_t Dummy_Gamepad::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("Joystick", m_JoystickOut);
  addOutPort("Buttons", m_ButtonsOut);
  m_Buttons.data.length(32);
  
  std::cout << "X, A, B, Yキーでボタン入力" << std::endl;
  std::cout << "↑,↓,→,←キーでジョイスティック入力" << std::endl;
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Dummy_Gamepad::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Dummy_Gamepad::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Dummy_Gamepad::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Dummy_Gamepad::onActivated(RTC::UniqueId ec_id)
{
	std::cout << "Activated" << std::endl;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Dummy_Gamepad::onDeactivated(RTC::UniqueId ec_id)
{
	std::cout << "Deactivated" << std::endl;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Dummy_Gamepad::onExecute(RTC::UniqueId ec_id)
{	
	//ボタン
	int val[32];
	for (int i = 0; i < 32; i++) {
		val[i] = 0;
	}
	if ((GetKeyState('X') & 0x8000) > 0) {
		val[0] = 1;
	}
	if ((GetKeyState('A') & 0x8000) > 0) {
		val[1] = 1;
	}
	if ((GetKeyState('B') & 0x8000) > 0) {
		val[2] = 1;
	}
	if ((GetKeyState('Y') & 0x8000) > 0) {
		val[3] = 1;
	}

	for (int i = 0; i < 32; i++) {
		m_Buttons.data[i] = val[i];
	}

	//ジョイスティック
	int vy = 0;
	int va = 0;

	if ((GetKeyState(VK_UP) & 0x8000) > 0 && (GetKeyState(VK_DOWN) & 0x8000) <= 0) {
		vy = 1000;
	}
	if ((GetKeyState(VK_DOWN) & 0x8000) > 0 && (GetKeyState(VK_UP) & 0x8000) <= 0) {
		vy = -1000;
	}
	if ((GetKeyState(VK_RIGHT) & 0x8000) > 0 && (GetKeyState(VK_LEFT) & 0x8000) <= 0) {
		va = -1000;
	}
	if ((GetKeyState(VK_LEFT) & 0x8000) > 0 && (GetKeyState(VK_RIGHT) & 0x8000) <= 0) {
		va = 1000;
	}

	m_Joystick.data.vy = vy;
	m_Joystick.data.va = va;

	m_JoystickOut.write();
	m_ButtonsOut.write();
	
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Dummy_Gamepad::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Dummy_Gamepad::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Dummy_Gamepad::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Dummy_Gamepad::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Dummy_Gamepad::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void Dummy_GamepadInit(RTC::Manager* manager)
  {
    coil::Properties profile(dummy_gamepad_spec);
    manager->registerFactory(profile,
                             RTC::Create<Dummy_Gamepad>,
                             RTC::Delete<Dummy_Gamepad>);
  }
  
};



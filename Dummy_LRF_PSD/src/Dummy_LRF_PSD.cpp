// -*- C++ -*-
/*!
 * @file  Dummy_LRF_PSD.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "Dummy_LRF_PSD.h"

// Module specification
// <rtc-template block="module_spec">
static const char* dummy_lrf_psd_spec[] =
  {
    "implementation_id", "Dummy_LRF_PSD",
    "type_name",         "Dummy_LRF_PSD",
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
Dummy_LRF_PSD::Dummy_LRF_PSD(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_LRFOut("LRF", m_LRF),
    m_PSDOut("PSD", m_PSD)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Dummy_LRF_PSD::~Dummy_LRF_PSD()
{
}



RTC::ReturnCode_t Dummy_LRF_PSD::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("LRF", m_LRFOut);
  m_LRF.ranges.length(40);
  addOutPort("PSD", m_PSDOut);
  m_PSD.data.length(3);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Dummy_LRF_PSD::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Dummy_LRF_PSD::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Dummy_LRF_PSD::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Dummy_LRF_PSD::onActivated(RTC::UniqueId ec_id)
{
	std::cout << "Activated" << std::endl;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Dummy_LRF_PSD::onDeactivated(RTC::UniqueId ec_id)
{
	std::cout << "Deactivated" << std::endl;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Dummy_LRF_PSD::onExecute(RTC::UniqueId ec_id)
{
	for (int i = 0; i < 40; i++) {
		double j = i * 0.05;
		m_LRF.ranges[i] = sqrt(2 * j * j - 2 * j + 1) * 500;
	}

	m_PSD.data[0] = 300;
	m_PSD.data[1] = 400;
	m_PSD.data[2] = 500;

	m_LRFOut.write();
	m_PSDOut.write();
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Dummy_LRF_PSD::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Dummy_LRF_PSD::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Dummy_LRF_PSD::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Dummy_LRF_PSD::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Dummy_LRF_PSD::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void Dummy_LRF_PSDInit(RTC::Manager* manager)
  {
    coil::Properties profile(dummy_lrf_psd_spec);
    manager->registerFactory(profile,
                             RTC::Create<Dummy_LRF_PSD>,
                             RTC::Delete<Dummy_LRF_PSD>);
  }
  
};



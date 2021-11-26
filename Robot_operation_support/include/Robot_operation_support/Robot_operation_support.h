// -*- C++ -*-
/*!
 * @file  Robot_operation_support.h
 * @brief ModuleDescription
 * @date  $Date$
 *
 * $Id$
 */

#ifndef ROBOT_OPERATION_SUPPORT_H
#define ROBOT_OPERATION_SUPPORT_H

#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
#include <timeapi.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <sys\types.h>
#include <process.h>
#include <Windows.h>
#include <sstream>
#include <time.h>
#include<iostream>

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="port_stub_h">
// </rtc-template>

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

using namespace RTC;
using namespace cv;

/*!
 * @class Robot_operation_support
 * @brief ModuleDescription
 *
 */
class Robot_operation_support
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  Robot_operation_support(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~Robot_operation_support();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /***
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  /***
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * 
   * - Name:  cameraID
   * - DefaultValue: 0
   */
  int m_cameraID;
  /*!
   * 
   * - Name:  width
   * - DefaultValue: 320
   */
  int m_width;
  /*!
   * 
   * - Name:  height
   * - DefaultValue: 240
   */
  int m_height;
  /*!
  *
  * - Name:  min_angle
  * - DefaultValue: 0
  */
  int m_min_angle;
  /*!
  *
  * - Name:  max_angle
  * - DefaultValue: 180
  */
  int m_max_angle;
  /*!
  *
  * - Name:  front_view
  * - DefaultValue: 0
  */
  int m_front_view;
  /*!
  *
  * - Name:  rear_view
  * - DefaultValue: 0
  */
  int m_rear_view;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::CameraImage m_Image;
  /*!
   */
  InPort<RTC::CameraImage> m_ImageIn;
  RTC::RangeData m_LRF;
  /*!
   */
  InPort<RTC::RangeData> m_LRFIn;
  RTC::TimedVelocity2D m_Gamepad;
  /*!
   */
  InPort<RTC::TimedVelocity2D> m_GamepadIn;
  RTC::TimedLongSeq m_PSD;
  /*!
  */
  InPort<RTC::TimedLongSeq> m_PSDIn;
  RTC::TimedDoubleSeq m_Latency;
  /*!
   */
  InPort<RTC::TimedDoubleSeq> m_LatencyIn;
  RTC::TimedLongSeq m_Buttons;
  /*!
   */
  InPort<RTC::TimedLongSeq> m_ButtonsIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::TimedDoubleSeq m_SendData;
  /*!
   */
  OutPort<RTC::TimedDoubleSeq> m_SendDataOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

 private:
	 int width;
	 int height;
	 int LRF_cnt,PSD_cnt,pad_cnt = 0;
	 int m_OutputQuarity = 95;
	 int key;
	 int min_dis;
	 int min_PSD;
	 int vy, va;
	 int x_coordinate[1000], y_coordinate[1000], line_color[1000];
	 int PSD_data[3];

	 bool Buttons_flg[32];
	 bool Buttons_switch[32];
	 string str_vy, str_va, str_min_dis, str_width, str_height;
	 IplImage* m_ImageBuf;
	 cv::Mat Tmp_Image;

	 //void Image_capture();


  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

};


extern "C"
{
  DLL_EXPORT void Robot_operation_supportInit(RTC::Manager* manager);
};

#endif // ROBOT_OPERATION_SUPPORT_H

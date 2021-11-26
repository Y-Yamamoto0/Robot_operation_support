// -*- C++ -*-
/*!
 * @file  Robot_operation_support.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "Robot_operation_support.h"

cv::Mat TmpImage;
cv::Mat m_imageBuff;
cv::VideoCapture cap(0);
std::ostringstream oss;
std::stringstream ss_vy;
std::stringstream ss_va;
std::string str;

int face[] = { cv::FONT_HERSHEY_SIMPLEX, cv::FONT_HERSHEY_PLAIN, cv::FONT_HERSHEY_DUPLEX, cv::FONT_HERSHEY_COMPLEX,
cv::FONT_HERSHEY_TRIPLEX, cv::FONT_HERSHEY_COMPLEX_SMALL, cv::FONT_HERSHEY_SCRIPT_SIMPLEX,
cv::FONT_HERSHEY_SCRIPT_COMPLEX, cv::FONT_ITALIC };

// Module specification
// <rtc-template block="module_spec">
static const char* robot_operation_support_spec[] =
  {
    "implementation_id", "Robot_operation_support",
    "type_name",         "Robot_operation_support",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "Yamamoto",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.cameraID", "0",
    "conf.default.width", "320",
    "conf.default.height", "240",
	"conf.default.min_angle", "0",
	"conf.default.max_angle", "180",
	"conf.default.front_view", "0",
	"conf.default.rear_view", "0",

    // Widget
    "conf.__widget__.cameraID", "text",
    "conf.__widget__.width", "text",
    "conf.__widget__.height", "text",
	"conf.__widget__.min_angle", "text",
	"conf.__widget__.max_angle", "text",
	"conf.__widget__.front_view", "radio",
	"conf.__widget__.rear_view", "radio",
    // Constraints
	"conf.__constraints__.front_view", "(0,1,2)",
	"conf.__constraints__.rear_view", "(0,1,2)",

    "conf.__type__.cameraID", "int",
    "conf.__type__.width", "int",
    "conf.__type__.height", "int",
	"conf.__type__.min_angle", "double",
	"conf.__type__.max_angle", "double",
	"conf.__type__.front_view", "int",
	"conf.__type__.rear_view", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Robot_operation_support::Robot_operation_support(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_ImageIn("Image", m_Image),
    m_LRFIn("LRF", m_LRF),
    m_GamepadIn("Gamepad", m_Gamepad),
	m_PSDIn("PSD", m_PSD),
    m_LatencyIn("Latency", m_Latency),
    m_ButtonsIn("Buttons", m_Buttons),
    m_SendDataOut("SendData", m_SendData)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Robot_operation_support::~Robot_operation_support()
{
}



RTC::ReturnCode_t Robot_operation_support::onInitialize()
{
	cap.release();
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("Image", m_ImageIn);
  addInPort("LRF", m_LRFIn);
  addInPort("Gamepad", m_GamepadIn);
  addInPort("PSD", m_PSDIn);
  addInPort("Latency", m_LatencyIn);
  addInPort("Buttons", m_ButtonsIn);
  
  // Set OutPort buffer
  addOutPort("SendData", m_SendDataOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("cameraID", m_cameraID, "0");
  bindParameter("width", m_width, "320");
  bindParameter("height", m_height, "240");
  bindParameter("min_angle", m_min_angle, "0");
  bindParameter("max_angle", m_min_angle, "180");
  bindParameter("front_view", m_front_view, "0");
  bindParameter("rear_view", m_rear_view, "0");
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Robot_operation_support::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Robot_operation_support::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Robot_operation_support::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Robot_operation_support::onActivated(RTC::UniqueId ec_id)
{
	std::cout << "Activated" << std::endl;
	for (int i = 0; i < 32; i++) {
		Buttons_flg[i] = false;
		Buttons_switch[i] = true;
	}
	for (int i = 0; i < 1000; i++) {
		x_coordinate[i] = 0;
		y_coordinate[i] = 0;
	}
	for (int i = 0; i < 3; i++) {
		PSD_data[i] = 1500;
	}
	min_dis = 4000;
	min_PSD = 1500;
	va = 0;
	vy = 0;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Robot_operation_support::onDeactivated(RTC::UniqueId ec_id)
{
	// カメラ用メモリの開放
	cv::destroyAllWindows();
	std::cout << "Deactivated" << std::endl;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t Robot_operation_support::onExecute(RTC::UniqueId ec_id)
{
	LRF_cnt += 1;
	PSD_cnt += 1;
	pad_cnt += 1;
	if (LRF_cnt >= 10) {		//一定カウント以上センサの読み込みがない場合データ表示を停止
		for (int i = 0; i < 1000; i++) {
			x_coordinate[i] = 0;
			y_coordinate[i] = 0;		
		}
		min_dis = 4000;
	}
	if (PSD_cnt >= 10) {
		for (int i = 0; i < 3; i++) {
			PSD_data[i] = 1500;
		}
		min_PSD = 1500;
	}

	if (m_GamepadIn.isNew()) {
		m_GamepadIn.read();	//ジョイスティック入力量読み込み

		vy = m_Gamepad.data.vy;
		va = -(m_Gamepad.data.va);

		pad_cnt = 0;

		//str_vy = std::to_string(vy);	//速度指令値表示用
		//str_va = std::to_string(va);

	}
	if (m_ButtonsIn.isNew()) {
		m_ButtonsIn.read();
		for (int i = 0; i < 32; i++) {
			if (Buttons_flg[i] == false && m_Buttons.data[i] != 0) {

				if (Buttons_switch[i] == false)Buttons_switch[i] = true;
				else Buttons_switch[i] = false;

				Buttons_flg[i] = true;
			}
			else if (Buttons_flg[i] == true && m_Buttons.data[i] == 0)Buttons_flg[i] = false;
		}
	}
	
	if (m_LRFIn.isNew()) {
		m_LRFIn.read();	//LRF距離データ読み込み
		LRF_cnt = 0;
		min_dis = 4000; //4mURGの最大距離（初期値）
		bool flg = false; //URGの距離データがすべて4[m]以上のときm_ConvertRangeData.ranges[i]にはすべて0がReadされる
		
		for (int i = 0; i < m_LRF.ranges.length(); i++)
		{
			x_coordinate[i] = 0;
			y_coordinate[i] = 0;

			if (20 < m_LRF.ranges[i] && m_LRF.ranges[i] < 700 && Buttons_switch[0] == true)//20mmより大きい値のみで計算することで誤ったデータを捨てる
			{
				double x_value = m_LRF.ranges[i] * cos(((((m_max_angle - m_min_angle)*(M_PI / 180)) / m_LRF.ranges.length()) * i) + m_min_angle*(M_PI / 180));	//ワールド座標系での各データの座標
				double y_value = m_LRF.ranges[i] * sin(((((m_max_angle - m_min_angle)*(M_PI / 180)) / m_LRF.ranges.length()) * i) + m_min_angle*(M_PI / 180));
				y_value *= 0.4;	//y方向のみ縮小する

				x_value *= 0.4;	//カメラ上の座標に変換
				y_value *= 0.4;
				x_value += width * 0.5;
				y_value += height * 0.83;
				x_value -= (x_value - (width  * 0.5)) * 2;	
				y_value -= (y_value - (height * 0.83)) * 2;

				x_coordinate[i] = (int)x_value;	//cv::line関数に用いるため、double型をint型に変換
				y_coordinate[i] = (int)y_value;
				
				line_color[i] = 255;								//500~700mmで黄色表示
				if (m_LRF.ranges[i] >= 300 && m_LRF.ranges[i] <= 500)line_color[i] = ((m_LRF.ranges[i] - 300.0)*(255.0 / 200.0));	//300~500mmで黄-赤グラデーション
				else if (m_LRF.ranges[i] < 300)line_color[i] = 0;	//~300mmで赤色表示
			}
			if (20 < m_LRF.ranges[i] && m_LRF.ranges[i] < min_dis) 
			{
				min_dis = m_LRF.ranges[i];
				str_min_dis = std::to_string(min_dis);	
				flg = true;
			}
		}
	}
	if (m_PSDIn.isNew()) {
		m_PSDIn.read();	//PSD距離データ読み込み
		PSD_cnt = 0;
		min_PSD = 1500;
		for (int i = 0; i < 3; i++) {
			PSD_data[i] = m_PSD.data[i];
			if (20 < m_PSD.data[i] && m_PSD.data[i] < min_PSD) {
				min_PSD = m_PSD.data[i];
			}
		}
	}

	if (m_ImageIn.isNew()) {
		//InPortから画像を読み込む
		m_ImageIn.read();
		m_imageBuff.create(cv::Size(m_Image.width, m_Image.height), CV_8UC3);
		//std::cout << "Read image " << std::endl;

		//画像のサイズが異なる場合は、画像を更新
		if (width != m_Image.width && height != m_Image.height) {
			width = m_Image.width;
			height = m_Image.height;

			//str_width = std::to_string(width);	//画像サイズ表示用
			//str_height = std::to_string(height);

			//画像のメモリ領域を確保
			m_ImageBuf = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
		}

		if (width != 0 && height != 0 && m_ImageBuf != NULL) {
			//画像をコピー
			memcpy(m_ImageBuf->imageData, (void*)&(m_Image.pixels[0]), m_Image.pixels.length());
		}
	}

	char *pWindowName = "Robot_camera";

	cv::Mat Robo_img = cvarrToMat(m_ImageBuf);
	cv::namedWindow(pWindowName, CV_WINDOW_AUTOSIZE);

	//Buttons_switch毎に表示
	//車幅延長線
	if (Buttons_switch[2] == true) {
		cv::line(Robo_img, Point(240, 480), Point(280, 360), Scalar(255, 255, 0), 2);
		cv::line(Robo_img, Point(280, 360), Point(360, 360), Scalar(255, 255, 0), 2);
		cv::line(Robo_img, Point(360, 360), Point(400, 480), Scalar(255, 255, 0), 2);
	}
	//LRF距離データ
	if (Buttons_switch[0] == true) {
		if (m_front_view == 0) {	//円弧形表示
			for (int i = 0; i < 1000;i++) {
				if( x_coordinate[i] != 0){
					cv::line(Robo_img, Point(width * 0.5, height * 0.83), Point(x_coordinate[i], y_coordinate[i]), Scalar(0, line_color[i], 255), 1);
					if (i != 0 && x_coordinate[i - 1] != 0) {
						cv::line(Robo_img, Point(x_coordinate[i - 1], y_coordinate[i - 1]), Point(x_coordinate[i], y_coordinate[i]), Scalar(0, line_color[i], 255), 3);
					}
				}			
			}
		}
		else if (m_front_view == 1) {	//大きな矢印の表示
			if(min_dis < 700){
				int LRF_color = 255;
				if (min_dis >= 300 && min_dis < 500)LRF_color = ((min_dis - 300.0)*(255.0 / 200.0));
				else if (min_dis < 300)LRF_color = 0;
				cv::line(Robo_img, Point(320,  50), Point(480, 130), Scalar(0, LRF_color, 255), 3);
				cv::line(Robo_img, Point(480, 130), Point(400, 130), Scalar(0, LRF_color, 255), 3);
				cv::line(Robo_img, Point(400, 130), Point(400, 200), Scalar(0, LRF_color, 255), 3);
				cv::line(Robo_img, Point(400, 200), Point(240, 200), Scalar(0, LRF_color, 255), 3);
				cv::line(Robo_img, Point(240, 200), Point(240, 130), Scalar(0, LRF_color, 255), 3);
				cv::line(Robo_img, Point(240, 130), Point(160, 130), Scalar(0, LRF_color, 255), 3);
				cv::line(Robo_img, Point(160, 130), Point(320,  50), Scalar(0, LRF_color, 255), 3);
				std::cout << "color:" << LRF_color << std::endl;
			}
		}
		else if (m_front_view == 2) {	//文字による警告表示
			if (min_dis < 700) {
				int LRF_color = 255;
				if (min_dis >= 300 && min_dis < 500)LRF_color = ((min_dis - 300.0)*(255.0 / 200.0));
				else if (min_dis < 300)LRF_color = 0;
				cv::putText(Robo_img, "Approaching obstacles", cv::Point(40, 100), face[0], 1.6, cv::Scalar(0, LRF_color, 255), 2, CV_AA);
				cv::putText(Robo_img, "in front!", cv::Point(200, 160), face[0], 1.6, cv::Scalar(0, LRF_color, 255), 2, CV_AA);
			}
		}
	}
	//ジョイスティック入力量
	if (Buttons_switch[1] == true) {
		cv::rectangle(Robo_img, Point(39, 9), Point(51, 81), Scalar(0, 0, 0), CV_FILLED);
		cv::rectangle(Robo_img, Point(9, 39), Point(81, 51), Scalar(0, 0, 0), CV_FILLED);
		cv::rectangle(Robo_img, Point(40, 10), Point(50, 80), Scalar(150, 150, 150), CV_FILLED);
		cv::rectangle(Robo_img, Point(10, 40), Point(80, 50), Scalar(150, 150, 150), CV_FILLED);
		if (pad_cnt < 10) {		
			double vy_value = ((double)-vy * 0.035) + 45.0;
			double va_value = ((double) va * 0.035) + 45.0;
			int vy_coord = (int)vy_value;
			int va_coord = (int)va_value;
			cv::line(Robo_img, Point(45, 45), Point(va_coord, vy_coord), Scalar(255, 0, 0), 6);
		}		
		/*入力量の数値、画像サイズを表示
		cv::putText(Robo_img, "straight: " + str_vy, cv::Point(10, 20), face[1], 1.3, cv::Scalar(0, 0, 0), 2, CV_AA);
		cv::putText(Robo_img, "straight: " + str_vy, cv::Point(10, 20), face[1], 1.3, cv::Scalar(255, 255, 255), 1, CV_AA);
		cv::putText(Robo_img, "turn    : " + str_va, cv::Point(10, 50), face[1], 1.3, cv::Scalar(0, 0, 0), 2, CV_AA);
		cv::putText(Robo_img, "turn    : " + str_va, cv::Point(10, 50), face[1], 1.3, cv::Scalar(255, 255, 255), 1, CV_AA);
		
		cv::putText(Robo_img, "min_distance: " + str_min_dis + " [mm] ", cv::Point(20, 80), face[1], 1.2, cv::Scalar(0, 0, 200), 1, CV_AA);
		cv::putText(Robo_img, "camera_size: " + str_width + "*" + str_height, cv::Point(20, 110), face[1], 1.2, cv::Scalar(0, 0, 200), 1, CV_AA);
		*/
	}
	//PSD距離データ
	if (Buttons_switch[3] == true) {
		if (m_rear_view == 0) {		//個別に矢印を表示
			for (int i = 0; i < 3; i++) {
				if (PSD_data[i] < 700) {
					int PSD_color = 255;
					if (PSD_data[i] >= 300 && PSD_data[i] < 500)PSD_color = ((PSD_data[i] - 300.0)*(255.0 / 200.0));
					else if (PSD_data[i] < 300)PSD_color = 0;
					if (i == 0) {
						cv::arrowedLine(Robo_img, Point(230, 380), Point(200, 470), Scalar(0, PSD_color, 255), 3, 8, 0, 0.2);
					}
					else if (i == 1) {
						cv::arrowedLine(Robo_img, Point(320, 380), Point(320, 470), Scalar(0, PSD_color, 255), 3, 8, 0, 0.2);
					}
					else if (i == 2) {
						cv::arrowedLine(Robo_img, Point(410, 380), Point(440, 470), Scalar(0, PSD_color, 255), 3, 8, 0, 0.2);
					}
				}
			}
		}
		else if (m_rear_view == 1) {	//大きな矢印の表示
			if(min_PSD < 700){
				int PSD_color = 255;
				if (min_PSD >= 300 && min_PSD < 500)PSD_color = ((min_PSD - 300.0)*(255.0 / 200.0));
				else if (min_PSD < 300)PSD_color = 0;
				cv::line(Robo_img, Point(320, 440), Point(480, 360), Scalar(0, PSD_color, 255), 3);
				cv::line(Robo_img, Point(480, 360), Point(400, 360), Scalar(0, PSD_color, 255), 3);
				cv::line(Robo_img, Point(400, 360), Point(400, 290), Scalar(0, PSD_color, 255), 3);
				cv::line(Robo_img, Point(400, 290), Point(240, 290), Scalar(0, PSD_color, 255), 3);
				cv::line(Robo_img, Point(240, 290), Point(240, 360), Scalar(0, PSD_color, 255), 3);
				cv::line(Robo_img, Point(240, 360), Point(160, 360), Scalar(0, PSD_color, 255), 3);
				cv::line(Robo_img, Point(160, 360), Point(320, 440), Scalar(0, PSD_color, 255), 3);
			}
		}
		else if (m_rear_view == 2) {	//文字による警告
			if (min_PSD < 700) {
				int PSD_color = 255;
				if (min_PSD >= 300 && min_PSD < 500)PSD_color = ((min_PSD - 300.0)*(255.0 / 200.0));
				else if (min_PSD < 300)PSD_color = 0;
				cv::putText(Robo_img, "Approaching obstacles", cv::Point(40, 380), face[0], 1.6, cv::Scalar(0, PSD_color, 255), 2, CV_AA);
				cv::putText(Robo_img, "in rear!", cv::Point(200, 440), face[0], 1.6, cv::Scalar(0, PSD_color, 255), 2, CV_AA);
			}
		}
	}
	
	cv::imshow(pWindowName, Robo_img);
	int key = cv::waitKey(5);

	if (key == 115) {						//sが押されたとき
		cv::imwrite("img.png", TmpImage);	//フレーム画像を保存
	}

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Robot_operation_support::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Robot_operation_support::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Robot_operation_support::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Robot_operation_support::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Robot_operation_support::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void Robot_operation_supportInit(RTC::Manager* manager)
  {
    coil::Properties profile(robot_operation_support_spec);
    manager->registerFactory(profile,
                             RTC::Create<Robot_operation_support>,
                             RTC::Delete<Robot_operation_support>);
  }
  
};



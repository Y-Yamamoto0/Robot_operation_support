# Robot_operation_support

## 概要
- LRF距離データ，PSD距離データ，車幅延長線，ジョイスティック入力量をカメラ映像上にオーバーレイ表示する．

## 特徴
- ゲームパッドのボタンにより表示する情報を選択
- LRF距離データは表示タイプを選択
- LRF距離データは円弧形に表示する際にスキャン範囲を調整

## 使用機器
- 株式会社イクシス　iWs09（移動ロボット）
- Logicool Gamepad F310（ゲームパッド）
- Dell G5 15 5500（ロボット用PC）
- 北陽電機株式会社　URG-04LX（LRF）
- シャープ株式会社　2Y0A02（PSD）
- オプト株式会社　NM22-N（広角カメラ）
## 開発環境
- Windows 10 64bit
- OpenRTM-aist 1.1.2
- Visual Studio 2015 (VC14)
- Phyton 2.7.10
- CMake-3.5.2
- Eclipse SDK Version: Luna SR2 (4.4.2)  
修正BSDライセンスとする．研究用途かつ利用者の責任下でご使用ください．

## コンポーネント群
### 新規RTC
- Robot_operation_supportRTC：操作情報をカメラ映像上にオーバーレイ表示するRTC．
#### 動作確認時
- Dummy_GamepadRTC：動作確認時にキーボード入力によりジョイスティック入力量およびボタン入力量を送信するRTC．ジョイスティック入力量はTimedVelocity2D型で出力し、ボタン入力はTimedLongSeq型で出力する．
- Dummy_LRF_PSDRTC：動作確認時にLRF・PSD距離データの一定値を送信するRTC．LRF距離データはRangeData型，PSD距離データはTimedLongSeq型で出力する．

### 既存のRTC
- OpenCVCameraRTC：カメラ映像をRobot_operation_supportへ送るRTC．OpenRTMに入っているRTCを用いる．CameraImage型で出力する．
#### 遠隔操作時
- screen_captureRTC：Robot_operation_supportRTCによって表示されたカメラ映像をキャプチャするRTC．CameraImage型で出力する．
- CameraClientRTC：カメラ映像をRSNP通信によってサーバに送信するRTC．CameraImage型を受信する．
- URGRTC：LRF距離データを取得するRTC．RangeData型で出力する．
- UrgControllerRTC：LRF距離データを間引きするRTC．RangeData型で受信・出力を行う．
- RobotClient_ver2RTC：ゲームパッドのジョイスティック入力量およびボタン入力を，RSNP通信によりサーバから取得するRTC．ジョイスティック入力量はTimedVelocity2D型で出力し，ボタン入力はTimedLongSeq型で出力する．
- RTC_iWs09RTC：移動ロボットの動作とPSD距離データの取得を行うRTC．移動量はTimedVelocity2D型を受信し，PSD距離データはTimedLongSeq型で出力する．
****
RTMコンテスト2021 

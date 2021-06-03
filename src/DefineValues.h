#define MYDEBUG

// ============================================================
// 펌웨어 버전 & PCB 버전
// ============================================================
#define FIRMWARE_VERSION "1.1.2"
#define PCB_VERSION 2

// ============================================================
// 현재 모드
// ============================================================
#define MODE_PORTABLE 1
#define MODE_ONLINE 2
#define MODE_AP 3
#define MODE_UPDATE 4

// ============================================================
// 현재 상태
// ============================================================
#define STATE_SOS 0
#define STATE_FIRE 1
#define STATE_COVER 2
#define STATE_NORMAL 12

// ============================================================
// GPIO
// ============================================================
#define GPIO_ADC_SENSOR A0
#define GPIO_MULTIPLEXER_0 16
#define GPIO_MULTIPLEXER_1 4
#define GPIO_SOS_BUTTON 0
#define GPIO_LED_R 12
#define GPIO_LED_G 13
#define GPIO_LED_B 15
#define GPIO_SOUND_IS_PLAYING 5
#define GPIO_I2C_SDA 2
#define GPIO_I2C_SCL 14
#define GPIO_RX 3

// ============================================================
// Sound
// ============================================================
#define SOUND_ALL_SYSTEM_ACTIVITY 0    // All System Activity
#define SOUND_SIREN 1                  // 사이랜
#define SOUND_BEEP 4                   // 비프음
#define SOUND_SENSOR_MODE 5            // 센서 사용 모드 변경
#define SOUND_EPPROM_CLEAR 6           // EEPROM 초기화 비프음
#define SOUND_SOS_BUTTON 10            // SOS버튼
#define SOUND_COVER_OPEN 20            // 커버오픈
#define SOUND_FIRE_DETECT 30           // 화재 감지
#define SOUND_FIRE_NEARBY 40           // 인접지역 화재
#define SOUND_DISASTER_ALERT 50        // 재난 경고
#define SOUND_DUST_BAD 59              // 미세먼지
#define SOUND_DUST_NORMAL 60           // 미세먼지
#define SOUND_DUST_GOOD 61             // 미세먼지
#define SOUND_WIFI_UNSTABLE 69         // WIFI 불안정
#define SOUND_WIFI_CONNECTED 70        // WIFI 연결됨
#define SOUND_PORTABLE_MODE 79         // 휴대모드
#define SOUND_LOW_BATTERY 80           // 배터리 방전
#define SOUND_REPLACE_EXTINGUISHER 81  // 소화기 교체
#define SOUND_STOLEN_ALERT 82          // 도난 경고
#define SOUND_WARNING_OFF 83           // 경고 해제
#define SOUND_UPDATE_COMPLATED 89     // 업그레이드 완료
#define SOUND_UPDATE_BEGIN 90         // 업그레이드 시작

// ============================================================
// I2C
// ============================================================
#define I2C_BITMASK 0x8000
#define I2C_CMD_LOAD 0x9400
#define I2C_CMD_PLAY 0x9800
#define I2C_CMD_PU1 0xA400
#define I2C_CMD_PU2 0xA800
#define I2C_CMD_PD1 0xB400
#define I2C_CMD_PD2 0xB800
#define I2C_CMD_VOL 0x4400
#define I2C_CMD_VOL_UP 0x4800
#define I2C_CMD_VOL_DN 0x5400
#define I2C_CMD_PAUSE 0x6400
#define I2C_CMD_RESUME 0x6800

// ============================================================
// LED
// ============================================================
#define RED 0
#define BLUE 1
#define VIOLET 2
#define WHITE 113
#define BLACK 139
#define CUSTOMPINK 140
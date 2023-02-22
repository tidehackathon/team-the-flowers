# DIRECTORIES
WORKDIR = 'D:\\PROJECTS\\Projects_Coding\\NATO_TIDE_2023\\team-the-flowers\\flora_nav\\'
BIN_DIR = WORKDIR + 'bin\\'
LOG_DIR = WORKDIR + 'log\\'
DATA_DIR = WORKDIR + 'data\\'

CALC_DATA_DIR = DATA_DIR + 'calc\\'
RAW_DATA_DIR = DATA_DIR + 'raw\\'

# RAW DATA FILES
GPS_POS_FILE  = RAW_DATA_DIR + 'gps_pos.csv'
GPS_VEL_FILE  = RAW_DATA_DIR + 'gps_vel.csv'
IMU_GYRO_FILE = RAW_DATA_DIR + 'imu_gyro.csv'
IMU_ACC_FILE  = RAW_DATA_DIR + 'imu_acc.csv'
IMU_ALT_FILE  = RAW_DATA_DIR + 'imu_alt.csv'
NAV_BEAR_FILE = RAW_DATA_DIR + 'nav_bear.csv'

# BIN CALC FILES
CALC_VEL_PRG = BIN_DIR + 'vel_calc.exe'
CALC_POS_PRG = BIN_DIR + 'pos_calc.exe'

# LOGFILES
CALC_VEL_LOG = LOG_DIR + 'calc_vel.log'
CALC_POS_LOG = LOG_DIR + 'calc_pos.log'

# CSV LOGS
CALC_VEL_CSV_LOG = CALC_DATA_DIR + 'vel.csv'

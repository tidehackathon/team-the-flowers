import sys
import time

import pymodules.calc.calc as calc
import pymodules.sensors.gps as gps
import pymodules.sensors.imu as imu
import pymodules.sensors.nav as nav

REFRESH_RATE = 100  # [ms]

def get_initial_pos() -> tuple[float]:
    while True:
        if gps_sensor.is_online():
            return gps_sensor.get_position()


def init() -> None:
    # Sensors handlers
    global imu_sensor, gps_sensor, nav_sensor
    imu_sensor = imu.IMU()
    gps_sensor = gps.GPS()
    nav_sensor = nav.NAV()

    # Calculation module handler
    global calc_module    
    calc_module = calc.Calc()

def main() -> None:
    imu_sensor.update()
    gps_sensor.update()
    nav_sensor.update()
    
    act_pos = get_initial_pos()
    
    first = True
    
    while True:
        # Sensors update
        imu_sensor.update()
        gps_sensor.update()
        nav_sensor.update()
        
        if first:
            # Get sensor data
            prev_gyro_data = imu_sensor.get_gyro_data()
            prev_acc_data = imu_sensor.get_acc_data()
            prev_alt = imu_sensor.get_altitude()
            prev_bearing = nav_sensor.get_bearing()
            
            first = False
            continue
            
        
        # Get sensor data
        gyro_data = imu_sensor.get_gyro_data()
        acc_data = imu_sensor.get_acc_data()
        alt = imu_sensor.get_altitude()
        bearing = nav_sensor.get_bearing()
        
        # Calculations
        #   * velocity
        vel = calc_module.estimate_velocity(prev_gyro_data=prev_gyro_data, prev_acc_data=prev_acc_data, act_acc_data=acc_data)
        #   * GPS position
        # act_pos = calc_module.calculate_position(prev_pos=act_pos, prev_alt=prev_alt, act_alt=act_alt, act_vel=vel, bearing=bearing, t=REFRESH_RATE)
        
        prev_gyro_data  = gyro_data
        prev_acc_data   = acc_data
        prev_alt        = alt
        prev_bearing    = bearing
        
        if (imu_sensor._marker >= len(imu_sensor._acc_data)):
            break
        
        out = f'{round(((imu_sensor._marker / len(imu_sensor._acc_data)) * 100), 7)} %'
        print(f'Processing... {out}  ', end='\r')
        
        # time.sleep(REFRESH_RATE / 1000)
        
    print('Processing... Done.', end='\r')
        
        
if __name__ == '__main__':
    init()
    main()
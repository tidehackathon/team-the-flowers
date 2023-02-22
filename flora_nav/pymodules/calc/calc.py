import sys
import csv
import subprocess
import pymodules.conf.config as cfg

class Calc:
    def __init__(self) -> None:
        self.vel_prg = cfg.CALC_VEL_PRG # Velocity calculation program path
        self.pos_prg = cfg.CALC_POS_PRG # Position calculation program path
        
        self.vel_log = open(cfg.CALC_VEL_LOG, 'w') # Velocity calc program logfile
        self.pos_log = open(cfg.CALC_POS_LOG, 'w') # Position calc program logfile

        self.vel_csv_log = open(cfg.CALC_VEL_CSV_LOG, 'w')
        self.vel_csv_log.write('t,velX,velY,velZ,vel\n')
        self.acc_csv_log = open(cfg.CALC_ACC_CSV_LOG, 'w')
        self.acc_csv_log.write('t,accX,accY,accZ\n')
        self.acc_cmp_csv_log = open(cfg.CALC_ACC_CMP_CSV_LOG, 'w')
        self.acc_cmp_csv_log.write('t,accCmpX,accCmpY,accCmpZ\n')
        
        self._prev_state = 1
        self._marker = 0
    
    '''
     Estimate UAV velocity with IMU data
     params:
      - gyroscope data
      - accelerometer data
      - previous velocity
     Returns estimated actual velocity value as a float value on success or None on errors
     
             1           2           3
        prev_gx,    prev_gy,    prev_gz
        4           5           6
        prev_ax,    prev_ay,    prev_az
        7           8           9
        ax,         ay,         az
    '''
    def estimate_velocity(self, prev_gyro_data : dict, prev_acc_data : dict, act_acc_data : dict) -> float | None:
        # Build program arguments
        args =  [   str(prev_gyro_data["x"]),
                    str(prev_gyro_data["y"]),
                    str(prev_gyro_data["z"]),
                    str(prev_acc_data["x"]),
                    str(prev_acc_data["y"]),
                    str(prev_acc_data["z"]),
                    str(act_acc_data["x"]),
                    str(act_acc_data["y"]),
                    str(act_acc_data["z"]),
                    str(self._prev_state)
                ]
        # Build command
        cmd = [self.vel_prg] + args
        
        # print(args)
        
        # Execute calculation program 
        result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        
        # Save stderr stream to log file
        self.vel_log.write(f'{result.stderr.decode("utf-8")}\n')
        
        # print(f'Acc: [{act_acc_data["x"]}, {act_acc_data["y"]}, {act_acc_data["z"]}]')
        
        self.acc_csv_log.write(f'{self._marker},{act_acc_data["x"]},{act_acc_data["y"]},{act_acc_data["z"]}\n')
        
        if result.returncode == 0:
            res = result.stdout.decode('utf-8').split(',')
            
            accCmpX, accCmpY, accCmpZ = res[0], res[1], res[2]
            velx, vely, velz = res[3], res[4], res[5]
            vel = res[6]
            self._prev_state = int(res[7])
            
            self.acc_cmp_csv_log.write(f'{self._marker},{accCmpX},{accCmpY},{accCmpZ}\n')
            self.vel_csv_log.write(f'{self._marker},{velx},{vely},{velz},{vel}\n')
            
            # print(f'{result.returncode} : {res}')
            
            return vel
        
        elif result.returncode == 1:
            res = result.stdout.decode('utf-8').split(',')
            
            accCmpX, accCmpY, accCmpZ = res[0], res[1], res[2]
            self._prev_state = int(res[3])
            
            self.acc_cmp_csv_log.write(f'{self._marker},{accCmpX},{accCmpY},{accCmpZ}\n')
            
            # print(f'{result.returncode} : {res}')
            
        self._marker = self._marker + 1
        
        return None
    
    
    
    '''
     Calculate UAV GPS position with input data
     params:
      - previous GPS position (lattitude, longitude)
      - previous altitude [m]
      - actual altitude [m]
      - actual velocity [m/s]
      - UAV bearing [degrees]
      - time [ms]
     Returns calculated UAV GPS position as tuple : (lattitude, longitude) on success or None on errors
    '''
    def calculate_position(self, prev_pos : tuple, prev_alt : float, act_alt : float, act_vel : float, bearing : float, t : float) -> tuple[float] | None:
        # Build program arguments
        args =  [   str(prev_pos[0]),
                    str(prev_pos[1]), 
                    str(prev_alt), 
                    str(act_alt), 
                    str(act_vel), 
                    str(bearing), 
                    str(t)
                ]
        # Build command
        cmd = [self.pos_prg] + args
        
        # Execute calculation program 
        result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        
        # Save stderr stream to log file
        sys.stderr.write(result.stderr.decode('utf-8'))
        
        # If Success
        if result.returncode == 0:
            # Collect calculated result
            res = result.stdout.decode('utf-8').split(',')
            lat, lon = res[0], res[1]
            return (lat, lon)
        
        return None

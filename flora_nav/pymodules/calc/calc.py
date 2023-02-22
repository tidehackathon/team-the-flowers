import sys
import csv
import subprocess
import pymodules.conf.config as cfg

class Calc:
    def __init__(self) -> None:
        self.vel_prg = cfg.CALC_VEL_PRG # Velocity calculation program path
        self.pos_prg = cfg.CALC_POS_PRG # Position calculation program path
        
        self.vel_log = cfg.CALC_VEL_LOG # Velocity calc program logfile
        self.pos_log = cfg.CALC_VEL_LOG # Position calc program logfile

        self.vel_csv_log = open(cfg.CALC_VEL_CSV_LOG, 'w')
        self.vel_csv_log.write('t,velX,velY,velZ,vel\n')
        
        self._marker = 0
        
        # Logfiles clear
        f = open(self.vel_log, 'w')
        f.close()
        f = open(self.pos_log, 'w')
        f.close()
    
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
                ]
        # Build command
        cmd = [self.vel_prg] + args
        
        # print(args)
        
        # Execute calculation program 
        result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        
        # Save stderr stream to log file
        # sys.stderr.write(result.stderr.decode('utf-8'))
        
        if result.returncode == 0:
            res = result.stdout.decode('utf-8').split(',')
            velx, vely, velz = res[0], res[1], res[2]
            vel = res[3]
            
            self.vel_csv_log.write(f'{self._marker},{velx},{vely},{velz},{vel}\n')
            
            return vel
        
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

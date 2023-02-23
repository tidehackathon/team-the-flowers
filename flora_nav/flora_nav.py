import sys

import navigator.conf.config as cfg
from navigator.navigator import Navigator

def init() -> None:
    # Navigator module
    global navigator
    
    print('Navigator module : \t\tInit...')
    navigator = Navigator(cfg.GPS_ALWAYS_OFFLINE, cfg.GPS_CHECK_INTERVAL)
    print('  * Navigator module : \t\tOK')

def main() -> None:
    # GPS position logfile to future track visualization
    print('GPS position logfile : \t\tInit...')
    pos_logfile = open(cfg.CALC_POS_CSV, 'w')
    
    # GPS position (latitude, longitude)
    position = navigator.get_actual_gps_position()
    pos_logfile.write(f'{position[0]}, {position[1]}\n')
    print('  * GPS position logfile : \tOK\n')
    
    str_ind = 4999      # start index
    stp_ind = 12000     # stop index
    i = str_ind
    
    print('UAV log data is being processed:')
    while True:
        navigator.update()
        position = navigator.get_actual_gps_position()
        pos_logfile.write(f'{position[0]}, {position[1]}\n')
        
        i = i + 1
        if i >= stp_ind:
            break
        
        out = f'{round((((i - str_ind) / (stp_ind - str_ind)) * 100), 2)} %'
        print(f'  * Processing:  100 / {out}   ', end='\r')
        
    print('  * Processing:  100 / 100 %   ')
    print(i)

def clean() -> None:
    print('\nCleaning...')
    del navigator
    print('  * Cleaned.\n')

if __name__ == '__main__':
    init()
    try:
        main()
        
    except KeyboardInterrupt:
        clean()
        print('-- Manually Interrupted --\n')
        print('Simulation program ended up with Success.\n')
        sys.exit(0)
        
    except Exception as e:
        clean()
        print('-- Unexpected Exception --')
        print(type(e).__name__)
        print(e, end='\n\n')
        print('Simulation program ended up with Failure!\n')
        sys.exit(1)
        
    clean()
    print('Simulation program ended up with Success.\n')
    sys.exit(0)

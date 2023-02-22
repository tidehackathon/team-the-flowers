import sys
import csv
import subprocess

# GPS Calculation program path
GPS_PRG = ['./bin/grid_calc.exe']

# ** INPUT CONSTS **
CSVFILE = './data/gps_set_1.csv'
HEADERS = ['timestamp(ms)', 'lat', 'lon', 'alt', 'vel', 'bearing']

# ** OUTPUT CONSTS **
GPS_FILE = './data/out/data_org.txt'
CAL_FILE = './data/out/data_cal.txt'
PLTLAT_FILE = './data/out/data_lat_plot.csv'
PLTLON_FILE = './data/out/data_lon_plot.csv'

PLTLAT_HEADERS = "timestamp (ms),GPS_Lat,Cal_Lat\n"
PLTLON_HEADERS = "timestamp (ms),GPS_Lon,Cal_Lon\n"


# ** FUNCTIONS **

# Load data from `csvfile`
def load_data(csvfile : str) -> dict:
    rows = [] 
    with open(csvfile, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            rows.append(row)
    return rows

# Build specific dataset from data row
#  Dataset elements order:
#   < lat, lon, alt, vel, bear, t >
def build_dataset(row : dict) -> list:
    lat     = float(row[HEADERS[1]])
    lon     = float(row[HEADERS[2]])
    alt     = float(row[HEADERS[3]])
    vel     = float(row[HEADERS[4]])
    bear    = float(row[HEADERS[5]])
    t       = float(row[HEADERS[0]])
        
    dataset = [lat, lon, alt, vel, bear, t]
    return dataset

# Build args from two datasets for GPS calculation program
#  Args order:
#   < lat, lon, alt1, alt2, vel, bear, dt >
def build_args(dataset1 : list, dataset2 : list) -> list:
    lat = dataset1[0]
    lon = dataset1[1]
    alt1 = dataset1[2]
    alt2 = dataset2[2]
    vel = dataset1[3]
    bear = dataset1[4]
    t = dataset2[5] - dataset1[5]
    
    args = [str(lat), str(lon), str(alt1), str(alt2), str(vel), str(bear), str(t)]
    
    return args


# ** MAIN **

def main():
    # Load data
    data = load_data(CSVFILE)

    first = True                # first row is being processed
    error_occured = False       # error occured in previous iteration
    errors = 0                  # error codes counter
    err_seq = []                # error codes sequences (following errors)

    # Output files opening    
    gpsFile = open(GPS_FILE, 'w')
    calFile = open(CAL_FILE, 'w')
    pltlatFile = open(PLTLAT_FILE, 'w')
    pltlonFile = open(PLTLON_FILE, 'w')
    
    # Output plot files : headers insert
    pltlatFile.write(PLTLAT_HEADERS)
    pltlonFile.write(PLTLON_HEADERS)
    
    # Main loop
    for i in range(len(data) - 1):
        # Build dataset for this and next timestamp
        dataset1 = build_dataset(data[i])
        dataset2 = build_dataset(data[i+1])
        
        # if not first row and previous calculation ended up with success
        if not first and not error_occured:
            # apply calculated `lat` and `lon` for next calculation 
            dataset1[0] = res[0] # lat
            dataset1[1] = res[1] # lon
        
        first = False
        
        # Build args
        args = build_args(dataset1, dataset2)
        
        # Create command to run
        cmd = GPS_PRG + args
        # Run GPS calculation program with muted STDERR
        result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
        
        # if no error occured
        if result.returncode == 0:
            # Get result: (lat,lon)
            res = result.stdout.decode('utf-8').split(',')
            res[0], res[1] = round(float(res[0]), 7), round(float(res[1]), 7)
            
            '''
            print(f'ARGS: {args}')
            print(f'ORG:  {dataset2[0]}, {dataset2[1]}')
            print(f'CAL:  {res[0]}, {res[1]}\n')
            '''
            
            # Write results to output files
            gpsFile.write(f'{dataset2[0]}, {dataset2[1]}\n')
            calFile.write(f'{res[0]}, {res[1]}\n')
            pltlatFile.write(f'{dataset2[5]},{dataset2[0]},{res[0]}\n')
            pltlonFile.write(f'{dataset2[5]},{dataset2[1]},{res[1]}\n')
            
            error_occured = False
        else:
            # if error occured during previous calculation
            if error_occured:
                # increase last error sequence counter
                err_seq[-1] = err_seq[-1] + 1
            else:
                # create new sequence counter
                err_seq.append(1)
                
            errors = errors + 1
            error_occured = True

        # UI/UX loading marker
        out = str(round((i / len(data)) * 100, 2)).center(5, ' ') + " / 100 %"
        print(f'Dataset is being processed... {out}', end='\r')
    
    print(f'Dataset is being processed... 100 / 100 %   \n')
    
    # Summary
    print(f'Calculations:   {len(data) - 1}')
    print(f'Errors:         {errors}')
    print(f'Max error seq:  ', end='')
    if errors > 0:
        print(max(err_seq))
    else:
        print(0)        
    
    # Files closing
    gpsFile.close()
    calFile.close()
    pltlatFile.close()
    pltlonFile.close()

    sys.exit(0)

if __name__ == '__main__':
    main()
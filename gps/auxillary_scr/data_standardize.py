import csv

SET_TEST_FILE = "./data/gps_test_set.csv"
STANDARD_TEST_FILE = "./data/gps_test_set_standarized.csv"

HEADERS = ['timestamp(ms)', 'lat', 'lon', 'alt', 'vel', 'bearing']

def load_test_data() -> dict:
    rows = [] 
    with open(SET_TEST_FILE, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            rows.append(row)
    return rows

def standarize_data(data) -> dict:
    for row in data:
        row[HEADERS[1]] = row[HEADERS[1]][:-7] + '.' + row[HEADERS[1]][-7:] # Latitude
        row[HEADERS[2]] = row[HEADERS[2]][:-7] + '.' + row[HEADERS[2]][-7:] # Longitude
        row[HEADERS[3]] = row[HEADERS[3]][:-3] + '.' + row[HEADERS[3]][-3:] # Altitude
        vel = float(row[HEADERS[4]]) / 100
        row[HEADERS[4]] = str(vel)
    return data

def save_standarized_data(data) -> None:

    keys = data[0].keys()

    with open(STANDARD_TEST_FILE, 'w', newline='') as output_file:
        dict_writer = csv.DictWriter(output_file, keys)
        dict_writer.writeheader()
        dict_writer.writerows(data)

def main():
    data = load_test_data()
    data = standarize_data(data)
    save_standarized_data(data)

if __name__ == '__main__':
    main()
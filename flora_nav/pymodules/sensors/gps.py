import csv
import pymodules.conf.config as cfg

class GPS:
    def __init__(self) -> None:
        self._pos_data  = self._load_csv_data(cfg.GPS_POS_FILE)
        self._marker = 0
        
        self._lat = 0.0 # Longitude
        self._lon = 0.0 # Longitude
        self.online = True
    
    # *** Getters ***
    # Returns actual GPS position : tuple of (lattitude, longitudes)
    def get_position(self) -> tuple[float]:
        return (self._lat, self._lon)
    
    # *** Checkers ***
    # Returns GPS signal status
    def is_online(self) -> bool:
        return self.online
        
    # *** Measurements update ***
    def update(self):
        self._lat = float((self._pos_data[self._marker][1][:-7] + '.' + self._pos_data[self._marker][1][-7:])) # Lattitude
        self._lon = float((self._pos_data[self._marker][2][:-7] + '.' + self._pos_data[self._marker][2][-7:])) # Longitude
        self._marker = self._marker + 1
        
    def _load_csv_data(self, filepath : str):
        rows = [] 
        with open(filepath, newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                rows.append(list(row.values()))
        return rows
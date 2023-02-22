import csv
import pymodules.conf.config as cfg

class NAV:
    def __init__(self) -> None:
        self._data  = self._load_csv_data(cfg.NAV_BEAR_FILE)
        self._marker = 0
        
        self.bearing = 0.0
    
    # *** Getters ***
    # Returns UAV bearing in degrees unit <0, 360> as float value
    def get_bearing(self) -> float:
        return self.bearing
    
    # *** Measurements update ***
    def update(self):
        try:
            bearing = float(self._data[self._marker][1])
            self.bearing = bearing
        except ValueError:
             pass
        finally:
            self._marker = self._marker + 1
        
    def _load_csv_data(self, filepath : str):
        rows = [] 
        with open(filepath, newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                rows.append(list(row.values()))
        return rows
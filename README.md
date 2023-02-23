# team-the-flowers

## **The Flowers**

### - Filip Szpręgiel

### - Bartłomiej Szykuła

### - Piotr Targowski

### - Jakub Walczak

---

## Challenge

### Small UAV Navigation Concept

---

## Solution

Navigation concept is 

---

## Project Structure

    │   flora_nav.py    `Main program - scenario simulator`
    │
    ├───bin
    │       pos_calc.exe
    │       vel_calc.exe
    │
    ├───data
    │   │   2019-09-10-16-18-38.tlog
    │   │
    │   ├───calc `Calculated data`
    │   │
    │   ├───raw  `Raw sensors data`
    │   │
    │   └───test_set `Test dataset`
    │       ├───in
    │       │       test_set_normalized.csv
    │       │       test_set_normalized_reduced.csv
    │       │       test_set_raw.csv
    │       │
    │       └───out
    │               data_cal.txt
    │               data_lat_plot.csv
    │               data_lon_plot.csv
    │               data_org.txt        `Original GPS route`
    │
    ├───lib `C++ source code`
    │   ├───pos `Position calculation program`
    │   │       pos_calc.cpp
    │   │       pos_calc.hpp
    │   │
    │   └───vel `Velocity calculation program`
    │       │   vel_calc.cpp
    │       │   vel_calc.hpp
    │       │
    │       └───libs
    │               gravity_compensation.cpp
    │               gravity_compensation.hpp
    │               madgwick.c  `Madgwick filter implementation`
    │
    ├───log
    │       calc_pos.log
    │       calc_vel.log
    │
    ├───navigator   `Core module directory`
    │   │   navigator.py    `Core`
    │   │
    │   ├───AI_camp `Visual supporting module`
    │   │       google_map_display.py
    │   │       requirements.txt
    │   │       satellite_photos.py
    │   │       similarity_tests.py
    │   │
    │   ├───conf    `Configuration`
    │   │       config.py
    │   │
    │   ├───modules `on-board modules handler programs`
    │   │       compute.py  `Computation module program`
    │   │       gps.py      `GPS module handler program`
    │   │
    │   └───sensors `on-board sensor handler programs`
    │           imu.py  `IMU handler program`
    │           nav.py  `Compass sensor handler program`
    │           win.py  `Wind sensor handler program`
    │
    └───py-tools    `Supporting visualisation tools`
            google_map_display.py       `Visual routes comparizer`
            plot.py                     `Plot maker`
            requirements.txt            `requirements for py-tools`

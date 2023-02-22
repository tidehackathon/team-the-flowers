import tkinter
from tkintermapview import TkinterMapView

ORG_DATAFILE = './data/out/data_org.txt'
CAL_DATAFILE = './data/out/data_cal.txt'

def Google_Maps_display() -> None:
    root_tk = tkinter.Tk()

    root_tk.geometry(f"500x500")

    root_tk.title("Maps interpretor")

    map_widget = TkinterMapView(
        root_tk, width=600, height=400, corner_radius=0)

    map_widget.pack(fill="both", expand="true")

    # =========== our calculated data ===========================

    # map_widget.set_tile_server("https://mt0.google.com/vt/lyrs=s&hl=en&x={x}&y={y}&z={z}&s=Ga", max_zoom=18)
    
    data_from_list_our = loop_for_data(CAL_DATAFILE)
    map_widget.set_address(f"{data_from_list_our[0]}", marker=False)
    start_point_x, start_point_y = float(data_from_list_our[0].split(
        ', ')[0]), float(data_from_list_our[0].split(', ')[1])

    for i, row in enumerate(data_from_list_our):
        x, y = float(row.split(', ')[0]), float(row.split(', ')[1])

        uv_path = map_widget.set_path([(start_point_x, start_point_y), (x, y)
                                       ], color='blue', width='7')
        uv_path.add_position(x, y)
        start_point_x = x
        start_point_y = y

    #  =========== original data ===========================

    data_from_list = loop_for_data(ORG_DATAFILE)
    map_widget.set_address(f"{data_from_list[0]}", marker=False)
    start_point_x, start_point_y = float(data_from_list[0].split(
        ', ')[0]), float(data_from_list[0].split(', ')[1])

    for i, row in enumerate(data_from_list_our):
        x, y = float(row.split(', ')[0]), float(row.split(', ')[1])

        uv_path = map_widget.set_path([(start_point_x, start_point_y), (x, y)
                                       ], color='red', width='3')
        uv_path.add_position(x, y)
        start_point_x = x
        start_point_y = y

    root_tk.mainloop()


def loop_for_data(data_file_name: str) -> list:
    my_file = open(data_file_name, 'r')
    data = my_file.readlines()
    return data


if __name__ == "__main__":
    #########
    #  =========== data format ===============
    #
    #               deg_x_m1
    #               deg_y_m1
    #               deg_x_m2
    #               deg_y_m2
    #               .
    #               .
    #               .
    #               deg_x_mn
    #               deg_y_mn
    #
    # =========== data format ===============
    ##########
    Google_Maps_display()

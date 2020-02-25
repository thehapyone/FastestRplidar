from PyQt5.QtWidgets import QMainWindow, QWidget, QApplication
from PyQt5.QtCore import QThread, pyqtSignal, QObject
import pyqtgraph as pg
import sys
from source import FastestRplidar
import time
import numpy as np


class Lidar(QObject):
    new_sample = pyqtSignal(object)

    def __init__(self):
        super().__init__()
        # Initialize the library and creates and object
        self.lidar = FastestRplidar()

        # connects the lidar using the default port (tty/USB0)
        print('connecting to lidar...')
        self.lidar.connectlidar()
        print('done')
        # Starts the lidar motor
        self.lidar.startmotor(my_scanmode=2)
        # fetches the lidar scan data

    def get_data(self):
        while True:
            scan = self.lidar.get_scan_as_xy(filter_quality=True)
            self.new_sample.emit(scan)
            time.sleep(0.1)


class MainWin(QMainWindow):
    def __init__(self):
        super().__init__()
        self.plot_widget = pg.PlotWidget()
        self.setCentralWidget(self.plot_widget)
        self.lidar_widget = Lidar()
        self.lidar_thread = QThread()
        self.lidar_widget.moveToThread(self.lidar_thread)
        self.lidar_thread.started.connect(self.lidar_widget.get_data)

        self.scatter = pg.ScatterPlotItem()
        self.plot_widget.addItem(self.scatter)
        self.plot = self.plot_widget.getPlotItem()
        self.plot.setXRange(-6000, 6000)
        self.plot.setYRange(-6000, 6000)
        self.lidar_widget.new_sample.connect(self.plot_data)

        self.lidar_thread.start()
        self.show()

    def plot_data(self, data):
        self.blockSignals(True)
        as_np = np.asarray(data)
        y_data = -as_np[:, 0]
        x_data = -as_np[:, 1]

        self.scatter.clear()
        self.scatter.setData(x=x_data, y=y_data)
        self.blockSignals(False)

    def closeEvent(self, event):
        self.lidar_widget.lidar.stopmotor()
        super().closeEvent(self, event)


if __name__ == '__main__':
    app = QApplication([])
    window = MainWin()
    sys.exit(app.exec_())

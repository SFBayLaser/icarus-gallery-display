from datatypes.database import dataBase
from ROOT import evd
import pyqtgraph as pg


class wire(dataBase):

    """docstring for wire"""

    def __init__(self):
        super(wire, self).__init__()
        self._process = None

    def getPlane(self, plane):
        return self._process.getArrayByPlane(plane)


class recoWire(wire):

    def __init__(self, detectorConfig):
        print("In recoWire initialization, detectorConfig type:",type(detectorConfig))
        super(recoWire, self).__init__()
        print("  >>> initializing the DrawWire object")
        self._process = evd.DrawWire(detectorConfig._geometryCore,detectorConfig._detectorProperties)
        self._process.initialize()
        self._process.setInput(self._producerName)
        for plane in range(detectorConfig.nViews()):
            self._process.setYDimension(geom.readoutWindowSize(),plane)
            print(geom.readoutPadding())
            if geom.readoutPadding() != 0:
                self._process.setPadding(geom.readoutPadding(), plane)

    def setProducer(self, producer):
        self._producerName = producer
        if self._process is not None:
            self._process.setInput(self._producerName)


class rawDigit(wire):

    def __init__(self, detectorConfig):
        print("In rawDigit initialization, detectorConfig type:",type(detectorConfig))
        super(rawDigit, self).__init__()
        print("  >>> initializing the DrawRawDigit object")
        self._process = evd.DrawRawDigit(detectorConfig._geometryCore,detectorConfig._detectorProperties)
        for i in range(len(detectorConfig._pedestals)):
            self._process.setPedestal(detectorConfig._pedestals[i], i)
        self._process.initialize()
        if "boone" in detectorConfig.DetectorName():
            self._process.SetCorrectData(False)
        else:
            self._process.SetCorrectData(False)
        for plane in range(detectorConfig.Nplanes()):
            print(">>>> Setting y dim in wire.py for plane ",plane," to ",detectorConfig.readoutWindowSize())
            self._process.setYDimension(detectorConfig.readoutWindowSize(),plane)
            if detectorConfig.readoutPadding() != 0:
                self._process.setPadding(detectorConfig.readoutPadding(), plane)


    def setProducer(self, producer):
        self._producerName = producer
        if self._process is not None:
            self._process.setInput(self._producerName)
            
    def toggleNoiseFilter(self, filterNoise):
        self._process.SetCorrectData(filterNoise) 
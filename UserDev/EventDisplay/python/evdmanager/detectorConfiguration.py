
import ROOT
from ROOT import larutil, galleryfmwk
import numpy as np

import os

class detConfigBase(object):

    """docstring for geometry"""

    def __init__(self,geometryCore,detectorProperties,clockProperties):
        super(detConfigBase, self).__init__()
        self._geometryCore = geometryCore
        self._detectorProperties = detectorProperties
        self._clockProperties = clockProperties
        self._nViews = geometryCore.Nviews()
        self._nPlanes = geometryCore.Nplanes()
        self._tRange = detectorProperties.NumberTimeSamples()
        self._wRange = []
        self._time2Cm = 0.1
        self._wire2Cm = geometryCore.WirePitch()
        self._aspectRatio = self._wire2Cm / self._time2Cm
        self._levels = [(-15, 15), (-10, 30)]
        self._name = "null"
        self._offset = []
        self._halfwidth = geometryCore.DetHalfWidth()
        self._halfheight = geometryCore.DetHalfHeight()
        self._length = geometryCore.DetLength()
        self._haslogo = False
        self._logo = None
        self._path = os.path.dirname(os.path.realpath(__file__))
        self._logopos = [0,0]
        self._logoscale = 1.0
        self._triggerOffset = clockProperties.TriggerOffsetTPC()
        self._readoutWindowSize = detectorProperties.ReadOutWindowSize()
        self._planeOriginX = [-0.2, -0.6] 
        self._planeOriginXTicks = [-0.2/0.4, -0.6/0.4] 
        self._readoutPadding = 0
        self._timeOffsetTicks = 0
        self._timeOffsetCm = 0
        for plane in range(0, self._nPlanes):
            self._wRange.append(geometryCore.Nwires(plane))
            self._offset.append(0)


    def halfwidth(self):
       return self._halfwidth

    def halfheight(self):
       return self._halfheight

    def length(self):
       return self._length

    def Nviews(self):
        return self._nViews

    def Nplanes(self):
        return self._nPlanes

    def tRange(self):
        return self._tRange

    def wRange(self, plane):
        return self._wRange[plane]

    def getLevels(self, plane):
        return self._levels[plane]

    def aspectRatio(self):
        return self._aspectRatio

    def getBlankData(self, plane):
        return np.ones((self._wRange[plane], self._tRange))

    def wire2cm(self):
        return self._wire2Cm

    def time2cm(self):
        return self._time2Cm

    def DetectorName(self):
        return self._name

    def offset(self, plane):
        return self._offset[plane]

    def hasLogo(self):
        return self._haslogo

    def logo(self):
        return self._logo

    def logoScale(self):
        return self._logoscale

    def logoPos(self):
        return self._logopos

    def readoutWindowSize(self):
        return self._readoutWindowSize

    def readoutPadding(self):
        return self._readoutPadding

    def triggerOffset(self):
        return self._triggerOffset
    
    def planeOriginX(self, plane):
        return self._planeOriginX[plane]

    def timeOffsetTicks(self, plane):
        return self._timeOffsetTicks
        # return self._timeOffsetTicks + self._planeOriginXTicks[plane]

    def timeOffsetCm(self, plane):
        return self._timeOffsetCm

class detectorConfiguration(detConfigBase):

    def __init__(self,geometryCore,detectorProperties,clockProperties):
        super(detectorConfiguration, self).__init__(geometryCore,detectorProperties,clockProperties)
        self._defaultColorScheme = []
        # self._tRange = larutil.DetProperties.GetME().ReadOutWindowSize()

    def colorMap(self, plane):
        return self._defaultColorScheme[plane]


class icarusConfiguration(detectorConfiguration):

    def __init__(self,geometryCore,detectorProperties,clockProperties):
        # Try to get the values from the geometry file.  Configure for microboone
        # and then call the base class __init__
        super(icarusConfiguration, self).__init__(geometryCore,detectorProperties,clockProperties)

        self._levels = [(-40, 160), (-40, 160), (-80, 320)]
        # self._colorScheme =
        # self._time2Cm = 0.05515
        self._pedestals = [2040, 2048, 400]
        self._name = "ICARUS"
        self._logo = self._path + "/logos/uboone_logo_bw_transparent.png"
        self._logoRatio = 1.0
        self._haslogo = True
        self._logopos = [1250,10]
        self._logoscale = 0.1
        self._planeOriginX = [0.0, -0.3, -0.6] 
        self._planeOriginXTicks = [0.0, -0.3/self._time2Cm, -0.6/self._time2Cm] 
        # remove = larutil.DetProperties.GetME().TriggerOffset() \
        #           * larutil.GeometriaHelper.GetME().TimeToCm()
        # self._offset[:] = [x - remove for x in self._offset]
        self._defaultColorScheme = [(
            {'ticks': [(0, (22, 30, 151, 255)),
                       (0.33333, (0, 181, 226, 255)),
                       (0.47, (76, 140, 43, 255)),
                       (0.645, (0, 206, 24, 255)),
                       (0.791, (254, 209, 65, 255)),
                       (1, (255, 0, 0, 255))],
             'mode': 'rgb'})]
#            {'ticks': [(1, (22, 30, 151, 255)),
#                       (0.791, (0, 181, 226, 255)),
#                       (0.645, (76, 140, 43, 255)),
#                       (0.47, (0, 206, 24, 255)),
#                       (0.33333, (254, 209, 65, 255)),
#                       (0, (255, 0, 0, 255))],
#             'mode': 'rgb'})]
        self._defaultColorScheme.append(
            {'ticks': [(0, (22, 30, 151, 255)),
                       (0.33333, (0, 181, 226, 255)),
                       (0.47, (76, 140, 43, 255)),
                       (0.645, (0, 206, 24, 255)),
                       (0.791, (254, 209, 65, 255)),
                       (1, (255, 0, 0, 255))],
             'mode': 'rgb'})
        self._defaultColorScheme.append(
            {'ticks': [(0, (22, 30, 151, 255)),
                       (0.33333, (0, 181, 226, 255)),
                       (0.47, (76, 140, 43, 255)),
                       (0.645, (0, 206, 24, 255)),
                       (0.791, (254, 209, 65, 255)),
                       (1, (255, 0, 0, 255))],
             'mode': 'rgb'})

#        self._offset = []
#        for v in range(0, self._nViews):
#            # Set up the correct drift time offset.
#            # Offset is returned in terms of centimeters.
#
#            self._offset.append(
#                self.triggerOffset()
#                * self.time2cm()
#                - self.planeOriginX(v) )



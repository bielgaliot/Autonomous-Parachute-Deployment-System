# BMP

Works only with BMP 180, for which you'll need to have the correspondign library installed in your Arduino environment

This code obtains the altitude of the BMP using the mean of 5 datapoints. It compares it with the mean of the next set of datapoints to determine whether it is going up or down. When going down is true and the height is under a set value, it triggers a mosfet gate. This was used in 2017 in the context of the LSS mission of the UPCSP to open a parachute at a set altitude.

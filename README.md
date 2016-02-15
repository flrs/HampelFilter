Arduino Hampel Filter Library
=============================
This library helps to identify outliers in data on Arduino.

Description
-----------
The Hampel filter is a simple but effective filter to find outliers in data. It performs better than a median filter.

This filter is designed to be used as an Arduino library. As Arduino feeds data points into a buffer, the filter does the following:
   1. Calculate the median of the values in the buffer
   2. Determine the deviations of the values in the buffer from the median
   3. Check what the median of these deviations is
   4. Multiply this median of deviations with a user-defined threshold value
   5. Check the threshold against user-defined query values to find out if values are outliers

The source code contains comments that help understand the behavior of the library further.

Usage
-----
The following code is a simple usage example:
```arduino
HampelFilter dataBuffer = HampelFilter(0.00, 3, 3.50);

void loop() {
    dataBuffer.write(24.39);
    dataBuffer.write(-4.83);
    dataBuffer.write(2.05);

    if(dataBuffer.checkIfOutlier(6.23)) {
        Serial.println("6.23 is an outlier!");
    }
}

```

The sample sketch `writeValuesAndFilter.ino` comes with the library and explains how to use it.

Download
--------
An archive with the library can be downloaded from the [releases page](https://github.com/flrs/HampelFilter/releases).

Installation
------------
To install the Arduino Hampel Filter library, unzip the downloaded file to the `./Arduino/libraries` folder, so that the path of the `HampelFilter.h` file is similar to `./Arduino/libraries/HampelFilter/HampelFilter.h`. It is important that the files of the library are placed in a folder named `HampelFilter`.

Contribution
------------
I am happy about any contribution or feedback. Please let me know about your comments via the Issues tab on [GitHub](https://github.com/flrs/HampelFilter/issues).

Acknowledgements
----------------
The initial but almost entirely reworked code for this library comes from improvements of the Arduino RunningMedian library by rkail on the [Arduino Forum](http://forum.arduino.cc/index.php?topic=53081.msg1160999#msg1160999).

The implementation of the comb sort algorithm was taken from this [Wikibooks article](https://en.wikibooks.org/w/index.php?title=Algorithm_Implementation/Sorting/Comb_sort&oldid=3032443) (checked on 2016-02-11).

The following freely available documents contain useful information about Hampel filters:
- http://dimacs.rutgers.edu/Workshops/DataCleaning/slides/pearson2.pdf
    (checked on 2016-02-11)
- https://www.seedtest.org/upload/cms/user/presentation2Remund2.pdf
    (checked on 2016-02-11)

License
-------
The Arduino Hampel Filter Library is distributed under a [Simplified BSD License](https://github.com/flrs/HampelFilter/blob/master/LICENSE.md).

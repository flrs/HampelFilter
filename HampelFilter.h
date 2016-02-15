/*** Arduino Hampel Filter Library ***

This library implements a Hampel filter for numerical data on Arduino. A Hampel filter is useful to find outliers in data. The sensitivity of this Hampel filter and the size of the data window that the filter looks at are adjustable.

The filter works on any numerical values, like 2.02, 35.98 and -232.43, as long as they are between -255.00 and 255.00. The filter delivers two-digit precision.

Further information about the Hampel filter can be found here:
    (1) https://www.seedtest.org/upload/cms/user/presentation2Remund2.pdf
        (checked on 2016-02-11)
    (2) http://dimacs.rutgers.edu/Workshops/DataCleaning/slides/pearson2.pdf
        (checked on 2016-02-11)

Library written by Florian Roscheck
Simplified BSD license, all text above must be included in any redistribution
***/

#ifndef HampelFilter_h
#define HampelFilter_h

#include <inttypes.h>
#include <stddef.h>

class HampelFilter {
public:
	// constructor
	HampelFilter(float i_default_val, uint8_t i_window_size, float i_scaling_factor);

	// adjustable filter parameters
	uint8_t window_size;	// no. of data points in the filter buffer
	uint16_t scaling_factor;	// scaling factor setting the outlier detection threshold

	void write(float i_new_value_flt);	// add value to buffer
	float readMedian();					// read median
	float readMAD();					// read median absolute deviation (MAD)
	float readOrderedValue(int i_position_in_list); // read specific value from buffer
	bool checkIfOutlier(float i_qry_value);	// check if queried value is an outlier

private:
	// heap allocation of buffers
	int16_t* inbuffer;		// cyclic buffer for incoming values
	int16_t* sortbuffer;	// sorted buffer to determine median
	int16_t* difmedbuffer;	// sorted buffer of absolute differences to median

	uint8_t buffer_ptr;		// pointer to current value index of cyclic inbuffer
	uint8_t median_ptr;		// pointer to median value of sortbuffer and difmedbuffer

	int16_t readMedianInt();// read median as integer (123 instead of 1.23)
	int16_t readMADInt();	// read MAD as integer
	void combSort11(int16_t *input, size_t size); // perform comb sort on data
};

#endif

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

#include <HampelFilter.h>
#include <stdlib.h> // Include this to get the abs()

HampelFilter::HampelFilter(float i_default_val, uint8_t i_window_size, float i_scaling_factor){
        // no. of data points in the filter buffer
        window_size = i_window_size;

        // scaling factor determining the outlier detection threshold
        scaling_factor = (uint16_t)(i_scaling_factor*100.0);

        buffer_ptr = i_window_size;
        median_ptr = i_window_size/2;

        inbuffer = new int16_t[i_window_size];
        sortbuffer = new int16_t[i_window_size];
        difmedbuffer = new int16_t[i_window_size];

        // write default values to buffers
        uint8_t i = window_size;
        while(i>0) {
                i--;
                inbuffer[i] = i_default_val*100;
                sortbuffer[i] = i_default_val*100;
                difmedbuffer[i] = 0;
        }
}

// add value to filter buffers
void HampelFilter::write(float i_new_value_flt) {

        int16_t i_new_value = i_new_value_flt*100;

        // set pointer for writing to cyclic buffer
        if(buffer_ptr==0)
                buffer_ptr = window_size;
        buffer_ptr--;

        int16_t old_value = inbuffer[buffer_ptr];

        // if value does not change no need for action, quit function
        if(i_new_value==old_value)
                return;
        inbuffer[buffer_ptr] = i_new_value;

        // temporarily store old median value as we need it later
        int16_t old_median = readMedianInt();

        // find old value in sortbuffer so we can replace it with the new value
        uint8_t i = window_size;
        while(i>0) {
                i--;
                if(old_value == sortbuffer[i])
                        break;
        }
        sortbuffer[i] = i_new_value;

        // now that old value has been replaced, sort the sortbuffer
        combSort11(sortbuffer, window_size);

        if(old_median != readMedianInt()) {
                // if the new median value is different from the old one, we need to recalculate all MAD values in the difmedbuffer
                i = window_size;
                while(i>0) {
                        i--;
                        difmedbuffer[i] = abs(sortbuffer[i]-readMedianInt());
                }
        } else {
                // median value has not changed, so we just need to replace the old MAD value with the new one in the difmedbuffer
                i = window_size;
                old_value = abs(old_value-old_median);
                while(i>0) {
                        i--;
                        if(old_value == difmedbuffer[i])
                                break;
                }
                difmedbuffer[i] = abs(i_new_value-old_median);
        }

        // Now sort the difmedbuffer. Sorting is always required because MAD is an absolute value and does thus not follow sortbuffer order
        combSort11(difmedbuffer, window_size);
}

float HampelFilter::readMedian() {
        return readMedianInt()/100.0;
}

float HampelFilter::readMAD() {
        return readMADInt()/100.0;
}

// read value from sortbuffer
float HampelFilter::readOrderedValue(int i_position_in_list) {
        return sortbuffer[(uint8_t)i_position_in_list]/100.0;
}

bool HampelFilter::checkIfOutlier(float i_qry_value) {
        int32_t qry_val = i_qry_value*100;
        return (int32_t)abs(qry_val-(int32_t)readMedianInt())*100>((int32_t)scaling_factor*(int32_t)readMADInt());
}

// read median absolute deviation (MAD) as integer (123 instead of 1.23)
int16_t HampelFilter::readMADInt() {
        /* Explanation of formula below:
        As fractional numbers are stored as integers multiplication result needs to be divided by 10000 (1.23*1.23=1.51 vs. 123*123=15129). In Pearson's paper (reference (2) in the header), page 7, MAD scale estimate Sk is calculated via Sk=1.48*MAD. Therefore, Sk can be calculated as
            Sk = 1.48 * MAD * 1/10000 = 1.48*100 * MAD * 1/100
               = MAD * 148/100.
        To save resources on the microcontroller, division only by powers of 2 should be made. 148/100 is approximated by 48497/32768, whereby 32768=2^15. So Sk = MAD * 48497/32768. The division can be expressed as a bit shift.
        */
        return (int16_t)(((int32_t)difmedbuffer[median_ptr]*48544) >> 15);
}

int16_t HampelFilter::readMedianInt() {
        return sortbuffer[median_ptr];
}

/* Comb sort routine from https://en.wikibooks.org/w/index.php?title=Algorithm_Implementation/Sorting/Comb_sort&oldid=3032443 (checked on 2016-02-11)
Comb sort is picked as it is faster than bubble sort. Both methods work well on microcontrollers.
*/
void HampelFilter::combSort11(int16_t *input, size_t size)
{
        uint8_t i,j,top,switched=0,gap=size;
        int16_t tmp;
        while (1<gap || switched)
        { if ((gap=(size_t)((float)gap / 1.3f)) < 11)
                  if (9<=gap) gap=11;
                  else if (!gap) gap=1;
          for (switched=0,    top=size-gap, i=0; i<top; ++i)
          {   j=i+gap;
        if (input[i]<input[j])
        { tmp=input[i]; input[i]=input[j]; input[j]=tmp; switched=1; }  //swap
          } }
}

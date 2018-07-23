// Simple demonstration on Hampel filter library use to collect
// data and find outliers in it.

#include <HampelFilter.h> // include library

// Define the dataBuffer variable which will carry the data that
// we would like to run the Hampel filter on. The buffer is
// defined like
//    HampelFilter <A> = HampelFilter(<B>,<C>,<D>);
// where
//    <A>:  Name of buffer variable
//    <B>:  Default value, like 0.00
//    <C>:  Buffer size (= window size of filter),
//             this should be an odd number, like 9, 27 or 289
//    <D>:  Threshold for outlier detection, this needs some
//             tweaking depending on the data. As a general
//             rule: The smaller the value, the more aggresive
//             the threshold. A good starting value is 3.5.
//             The value cannot be smaller than 0 and not
//             greater than 655.
HampelFilter dataBuffer = HampelFilter(0.00, 11, 3.50);

void setup() {
  // Let us start the serial output so we can see what is going on
  Serial.begin(9600);
  Serial.println("--- Hampel Filter Test ---");
}

void loop() {
  // Add some values to the dataBuffer
  dataBuffer.write(1);
  dataBuffer.write(9);
  dataBuffer.write(-7);
  dataBuffer.write(-2);
  dataBuffer.write(3);
  dataBuffer.write(3);
  dataBuffer.write(14.0);
  dataBuffer.write(-45.02);

  // Values can be integers or floats. For floats, only
  // two decimals will be considered.
  dataBuffer.write(7.64);

  // The following value is read as "2.98" by the library
  dataBuffer.write(2.9866372);

  // Careful: Only values in the range from -255.0 to 255.0 can
  // be used by the library! It will mess up values if any values
  // exceed this range!
  dataBuffer.write(-255.0);

  // Now, let us read what the median value of the data in the
  // buffer is!
  Serial.print(" Median value in buffer is: ");
  Serial.println(dataBuffer.readMedian());

  // We can also check something called "median absolute deviation from
  // the median" (MAD). This value is important when the library checks
  // which point is an outlier.
  Serial.print(" The median absolute deviation (MAD) is: ");
  Serial.println(dataBuffer.readMAD());


  // Let us find some outliers!
  if(dataBuffer.checkIfOutlier(4.5)==true){
    Serial.println("4.5 is an outlier!");
  }else{
    Serial.println("4.5 is not an outlier.");
  }

  if(dataBuffer.checkIfOutlier(55.23)==true){
    Serial.println("55.23 is an outlier!");
  }else{
    Serial.println("55.23 is not an outlier.");
  }

  // In many cases you have an application with measurement values
  // values that slowly change over time. You would like
  // to capture that change, so you should continuously add
  // values to the dataBuffer, even points identified as outliers.
  // Because the oldest values in the dataBuffer get discarded, these
  // small changes can be captured. The filter can still find
  // outliers in the data. Let us simulate that!

  // Add some really high values and check what is perceived as
  // outlier
  Serial.println();
  Serial.println("Now we are adding some really high values:");
  float veryHighValues[] = {238, 229, 245, 232, 239.5, 244, 230.43, 243};
  for(int i=0;i<8;i++){
      dataBuffer.write(veryHighValues[i]);
      Serial.print("Adding ");
      Serial.print(veryHighValues[i]);
      Serial.print(". ");

      Serial.print(veryHighValues[i]);
      if(dataBuffer.checkIfOutlier(veryHighValues[i])==true){
        Serial.print(" is an outlier! ");
      }else{
        Serial.print(" is not an outlier. ");
      }
        if(dataBuffer.checkIfOutlier(4.5)==true){
          Serial.print("And 4.5 is an outlier! ");
        }else{
          Serial.print("And 4.5 is not an outlier. ");
        }
      Serial.print("And our MAD estimate is ");
      Serial.println(dataBuffer.readMAD());
  }

// end program
Serial.println();
Serial.println("End of test.");
delay(10000000);
}

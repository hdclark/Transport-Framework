//Simple_Backproj.cc - Simple, unfiltered parallel beam-source geometry reconstruction.
//
//This program takes a single detector reading (at a single orientation) and performs a basic backprojection.

#include <iostream>
#include <fstream>
#include <cmath>

#include <string>
#include <sstream>
#include <vector>

#include <Magick++.h>

#include <fftw3.h>
using namespace Magick;

fftw_complex *fft_in_forward, *fft_out_forward;
fftw_complex *fft_in_backward, *fft_out_backward;
fftw_plan fft_plan_forward, fft_plan_backward;



struct Complete_Data { //This represents a complete data set for a CT scan at a fixed angle. Add metadata here as needed.
    double theta;
    std::vector<double> events;
};


template <class T> T stringtoX(std::string text){
    std::stringstream ss(text);
    T temp;
    ss >> temp;
    return temp;
}

inline double  sinc(double a, double x){
    if((a*x) == 0.0) return 1.0/M_PI;
    return sin(a*x)/(M_PI*x);   //Normalized integration over (-inf,inf).
}

std::vector<double> Load_Detector_File(std::string strFilename){
    std::vector<double> detector_data;

    //Test that the file exists and can be opened.
    std::ifstream file;
    file.open(strFilename.c_str(), std::ios::in );
    file.close();
    if(!file.good()){
        std::cout << "Failed to open file '" << strFilename << "'" << std::endl;
        return detector_data;
    }
    file.open(strFilename.c_str(), std::ios::in );//| std::ios::binary);

    do{
        std::string strBuffer;
        getline(file,strBuffer);

        //Grab the first non-whitespace character.
        std::string::size_type pos = strBuffer.find_first_not_of(" \t");

        //If the line is empty, it is a comment line, EOF has been reached, or there is a 
        // problem reading the file, go to next line.
        if( file.good()
            && (pos != std::string::npos)
            && (strBuffer[pos] != '\n')
            && (strBuffer[pos] != '\r')
            && (strBuffer[pos] != 0)
            && (strBuffer[pos] != '#') 
          ){

            std::stringstream ss(strBuffer);

            //This line is then a non-empty, non-comment line of text which needs to be parsed.
            //We still need to ensure proper syntax.
            std::vector<std::string> thisline;

            //Populate the line vector with words.  Each word is an element of the vector.
            while( ss.good() ){
                std::string temp;
                ss >> temp;
                if(temp != "") thisline.push_back(temp);
            } 

            //There are two number. The first is an integer (from 0 to ...) denoting the bin number,
            // the second is the bin event count. This is the part we are interested in.
            if(thisline.size() == 2){
                detector_data.push_back( stringtoX<double>( thisline[1]) );
 
            }else{
                std::cout << "Unable to parse line. Quoted entries are: ";
                for(std::string &i : thisline) std::cout << "\"" << i << "\" ";
                std::cout << std::endl;
               //std::cout << " Error parsing file. Are there more than 2 numbers on each non-comment line?" << std::endl;

            }

        }//if( fail.good() ...

    }while(file.good());
    file.close();

    return detector_data;
}



void Write_Image(int W, int H, std::vector<std::vector<double> > thedata, std::string Filename){

    Image magicimg(Geometry(W, H), "white");
    magicimg.magick("jpeg");

    for(int i=0; i<W; ++i) for(int j=0; j<H; ++j){
        double R = thedata[i][j]; //[3*(j*W + i) + 0];
        double G = thedata[i][j]; //arr[3*(j*W + i) + 1];
        double B = thedata[i][j]; //arr[3*(j*W + i) + 2];

        magicimg.pixelColor(i,(H-1)-j, \
            Color(MaxRGB*(float)(R), \
                  MaxRGB*(float)(G), \
                  MaxRGB*(float)(B)));
    }

    //This call does all the writing.
    std::string path = "/tmp/Transport_Image.jpg";
    magicimg.write( path.c_str() );

    return ;
}


int to_pixel_coordinates_x(double x, double detector_width){
    double tempx = x + 0.5*detector_width;
    return (int)rint(tempx/1.0);
}

int to_pixel_coordinates_y(double y, double detector_width){
    double tempy = y + 0.5*detector_width;
    return (int)rint(tempy/1.0);
}


int main(int argc, char **argv){

    //Set up FFTW stuff.
    fft_in_forward = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 512);
    fft_out_forward = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 512);
    fft_plan_forward = fftw_plan_dft_1d(512, fft_in_forward, fft_out_forward, FFTW_FORWARD, FFTW_ESTIMATE);

    fft_in_backward = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 512);
    fft_out_backward = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 512);
    fft_plan_backward = fftw_plan_dft_1d(512, fft_in_backward, fft_out_backward, FFTW_BACKWARD, FFTW_ESTIMATE);


    //The 'experimental data' is collected into a vector of measurements at a fixed orientation. Each fixed orientation is 
    // called a 'Complete_Data' set. 
    std::vector<Complete_Data> data;

    //---------------------------------------------------------------------------------------------------
    //---------------------------------------- Data Gathering -------------------------------------------
    //---------------------------------------------------------------------------------------------------
    //First, read in the baseline (calibration) signal. We use this to subtract off the signal we see.
    struct Complete_Data baseline;
//    baseline.events = Load_Detector_File("/home/hal/Dropbox/Project - Transport/Calibrations/Detector_Xray_CT_N7599x10_baseline_500000_photons.plot");
    baseline.events = Load_Detector_File("/home/hal/Dropbox/Project - Transport/Calibrations/Detector_Xray_CT_N7599x2.5_baseline_5000000_photons.plot");
    baseline.theta  = 0.0;

    //Now read in the data. Pack it into a struct with the corresponding angle.
    {
      struct Complete_Data temp;
      temp.events = Load_Detector_File("/tmp/Transport_Detector.plot");
      temp.theta  = 0.0;

      data.push_back(temp);      
    }

    //A simple hack, to get better performance, for spherically-symmetric objects.
    for(double t = 2.0; t <= 360.0; t += 2.0){
      struct Complete_Data temp;
      temp.events = data[0].events;
      temp.theta  = t*2.0*M_PI/360.0;

      data.push_back(temp);
    }

/*
    {
      struct Complete_Data temp;
      temp.events = Load_Detector_File("/tmp/Transport_Detector.plot");
      temp.theta  = 0.5*M_PI;

      data.push_back(temp);
    }

    {
      struct Complete_Data temp;
      temp.events = Load_Detector_File("/tmp/Transport_Detector.plot");
      temp.theta  = M_PI;

      data.push_back(temp);
    }

    {
      struct Complete_Data temp;
      temp.events = Load_Detector_File("/tmp/Transport_Detector.plot");
      temp.theta  = 3.0*M_PI/2.0;

      data.push_back(temp);
    }
*/

    //Verify that all data is of the same dimension, no duplicate sets, sane values, etc..
    if(data.size() == 0){
        std::cout << "Issues reading data. Exiting." << std::endl;
        return -1;
    }
    size_t detector_cell_count = 0;  //The number of individual detector cells in a complete detector.
    detector_cell_count = baseline.events.size();
    for(size_t i=0; i<data.size(); ++i){
        if(data[i].events.size() != detector_cell_count){
            std::cout << "Attempting to mix data from detectors of varying size. Are you sure this is from the same simulation?" << std::endl;
            return -1;
        }
    }

    //---------------------------------------------------------------------------------------------------
    //------------------------------------------ Preprocessing ------------------------------------------
    //---------------------------------------------------------------------------------------------------

/*
    //Subtract the signal from the baseline.
    for(size_t i=0; i<data.size(); ++i){
        for(size_t j=0; j<detector_cell_count; ++j){
            data[i].events[j] = baseline.events[j] - data[i].events[j];
        }
    }
*/


    //Log normalization.
    for(size_t i=0; i<data.size(); ++i){
        for(size_t j=0; j<detector_cell_count; ++j){
            if( (data[i].events[j] <= 0.0) || (baseline.events[j] <= 0.0) ){
                data[i].events[j] = 0.0;
            }else{
                data[i].events[j] = log( baseline.events[j]/data[i].events[j]);
            }
        }
    }





/*
    //Invert the input from events to signal strength. Alternatively: post-invert. 
    for(size_t i=0; i<data.size(); ++i){
        double max = -1E99;
        for(size_t j=0; j<data[i].events.size(); ++j){
            if(data[i].events[j] > max) max = data[i].events[j];
        }
 
        if(max != -1E99){
            for(size_t j=0; j<data[i].events.size(); ++j){
                data[i].events[j] = max - data[i].events[j];
            }
        }
    }
*/



/*
    //Convolute the signal with a sinc function for filtered back-projection.
    double scale = 1.0 / static_cast<double>(detector_cell_count);                //This value is tuneable. It should match the filter point-spread function. I'm eyeballing this value...
    for(size_t i=0; i<data.size(); ++i){
        std::vector<double> newevents = data[i].events;
        for(size_t t=0; t<detector_cell_count; ++t){

            double thisbin = 0.0;
            for(size_t tau=0; tau<detector_cell_count; ++tau){    // tau < t instead?
                thisbin += data[i].events[tau] * sinc(scale, (static_cast<double>(t) - static_cast<double>(tau)));
            }
            newevents[t] = thisbin;
        }
        data[i].events = newevents;
    }
*/


/*
    //Signal injection: replace actual data with half-circle. Observe output!
    for(size_t i=0; i<data.size(); ++i){
        for(size_t j=0; j<detector_cell_count; ++j){
            const double x = static_cast<double>(j);
            if( (x <= 128.0) || (x >= 384.0) ){
                data[i].events[j] = 0.0;
            }else{
                data[i].events[j] = sqrt(1.0-(1.0/128.0)*(1.0/128.0)*(x-256.0)*(x-256.0));
            }
        }
    }
*/



    //Go to Fourier-space, apply a ramp filter, transform back, swap the data.
    for(size_t i=0; i<data.size(); ++i){
        //Fill the forward FFTW input array.
        for(size_t j=0; j<detector_cell_count; ++j){
            fft_in_forward[j][0] = data[i].events[j];
            fft_in_forward[j][1] = 0.0;
        }

        //Perform the FFT.
        fftw_execute(fft_plan_forward);

        //Grab the data. Filter it/transform as necessary and dump it into the reverse FFT input array.
        for(size_t j=0; j<detector_cell_count; ++j){

//------- This has been tested with the injected data and shown to work! -----------
            //Ramp filter.
            double f = (static_cast<double>(j)/static_cast<double>(detector_cell_count));  //Clamped frequency: [0:1]    ie.  |/|
            f = (f <= 0.5) ? f : 1.0 - f ;   // make into |/\|
            f *= 2.0;


            //f = 1.0 - f; // make into |\|  or |\/|        <----- not needed!

            double ramp_val;
            //Basic.
            ramp_val = f ;

//-------------------------------------------------------------

            //Sharp.
//            ramp_val = f + 2.0*(f*f*f/6.0) + (f*f*f*f*f/120.0); 


            //Smooth.
//            ramp_val = f - 2.0*(f*f*f/6.0) + (f*f*f*f*f/120.0);

            //Super-enhance. 
//            ramp_val = powf(4.0*f, 2.0);

            ramp_val = sqrt(2.0*f);

            ramp_val = pow(f,0.3);


            //This basically does not change anything in the final image.
            //double ramp_val = fabs((static_cast<double>(j)-0.5*static_cast<double>(detector_cell_count))/static_cast<double>(detector_cell_count));

            fft_in_backward[j][0] = ramp_val * fft_out_forward[j][0];
            fft_in_backward[j][1] = ramp_val * fft_out_forward[j][1];
        }
        
        //Perform the inverse FFT.
        fftw_execute(fft_plan_backward);


        //Take the output and stick it in the detector array.
        for(size_t j=0; j<detector_cell_count; ++j){
            data[i].events[j] = fft_out_backward[j][0];
            //data[i].events[j] = fft_out_backward[j][1];   // <----- these complex values should be ~zero. Check? (Should they be??)
        }

    }



    //Smooth out the data.
   {
    int NN = 2;  //Number of nearest neighbours to average over.
    for(int i=0; i<data.size(); ++i){
        std::vector<double> temp(detector_cell_count);
        for(int j=0; j<detector_cell_count; ++j){
            temp[j] = 0.0;
            double count = 0.0;
            for(int k = -NN; k <= NN; ++k){
                if(((j+k) >= 0) && ((j+k)<detector_cell_count)){  
                    count += 1.0;
                    temp[j] += data[i].events[j+k];
                }
            }

            //temp[j] /= count;
            temp[j] /= static_cast<double>(NN);
        }

        //Now swap the vectors.
        data[i].events = temp;
    }
   }



    //---------------------------------------------------------------------------------------------------
    //-------------------------------- Prepare the output containers ------------------------------------
    //---------------------------------------------------------------------------------------------------
    //Now make a 2d grid which is the same size as the detector.
    std::vector<std::vector<double> > image;   //The actual image.
    std::vector<std::vector<bool> >   mask;    //Used for avoiding duplicate accumulation in cells when raycasting.
    for(size_t i=0; i<detector_cell_count; ++i){
        std::vector<double> temp;
        std::vector<bool>   temp2;
        for(size_t j=0; j<detector_cell_count; ++j){
            temp.push_back(0.0);
            temp2.push_back(false);
        }
        image.push_back( temp );
        mask.push_back( temp2 );
    }


    //---------------------------------------------------------------------------------------------------
    //---------------------------------------- Backprojection -------------------------------------------
    //---------------------------------------------------------------------------------------------------
    //We treat the detector array as cells of unit width. Likewise, the image pixels are 1x1 units.
    //
    //Assumptions made here:
    // - Detector points at center of image. The center of the detector array always intersects the
    //   image array at the center point, which is the point of rotation.
    // - The detector is assumed to be a parallel beam setup. NO fan beam. NO cone beam.
    // - The center of the detector is in contact with a circle enscribing the image data.
    //
    // Under these assumptions, slices through the image data are defined by an angle and a distance 
    //  from the origin.

    double detector_width = static_cast<double>(detector_cell_count);
    double rmax = detector_width/sqrt(2);  // = sqrt( detector_width*detector_width + detector_height*detector_height);

    std::cout << "Running for set ";
    for(size_t k = 0; k < data.size(); ++k){
   
        std::cout <<  k <<  " ";
        std::cout.flush();
 
        //These define the unit vector aligned with the detector array.
        double unit_rot_x = cos( data[k].theta + M_PI/2.0 );
        double unit_rot_y = sin( data[k].theta + M_PI/2.0 );
    
        //These define the unit vector along the rays from the detector cells.
        double unit_trans_x = cos( data[k].theta );
        double unit_trans_y = sin( data[k].theta );
   
        //Zero the 'did we ray cast into this voxel yet?' mask. 
        for(size_t d1=0; d1<detector_cell_count; ++d1){
            for(size_t d2=0; d2<detector_cell_count; ++d2){
                mask[d1][d2] = false;
            }
        }

        for(size_t which_cell = 0; which_cell < detector_cell_count; ++which_cell){
        
            //Set the coordinates at the detector cell.
            double x = rmax*unit_trans_x; //Central cell.
            double y = rmax*unit_trans_y; //Central cell.

            //Mind the sign convention here! (Work it out in the first quadrant and you'll see where the negatives come from.
            x += ( (detector_width/2.0) - static_cast<double>( which_cell ) - 0.5)*(-unit_rot_x); //The 0.5 is to be in the center of the cell.
            y += (-(detector_width/2.0) + static_cast<double>( which_cell ) + 0.5)*unit_rot_y;    //The 0.5 is to be in the center of the cell.
        
            //Now, cast through the material by incrementing and dumping the image data.
            for(double inc = 0.0; inc < 2.0*rmax; inc += 0.3*1.0){
                //The proper dimension to cast toward is toward the origin from the cell point.    
                double this_x = x;
                double this_y = y;

                this_x -= inc*unit_trans_x;
                this_y -= inc*unit_trans_y;
        
                //This_x and this_y now need to be translated to image coordinates. Note that while x and y are in a full (four-quadrant) 
                // 2d space, the pixel coordinates occupy a single quadrant and must be integer. We will therefore lose corners for most angles.
                int i = to_pixel_coordinates_x(this_x, detector_width);
                int j = to_pixel_coordinates_y(this_y, detector_width);
            
//                std::cout << "Got pixel coordinates " << i << " " << j << std::endl;
            
                if((i >= 0) && (j >= 0) && (i < detector_cell_count) && (j < detector_cell_count) && (mask[i][j] == false)){ //We will *definately* get nonsensical values for i and j!
                    image[i][j] += data[k].events[ which_cell ];
                    mask[i][j]  = true;
                }
        
            }
        }

        //NOTE: checking the mask for missed cells would be largely useless. We are guaranteed to miss a loarge number of perhipheral pixels
 
    } //Loop over all orientations.

    std::cout << std::endl;

/*
    //0 degree backprojection.
    for(size_t i=0; i<detector_cell_count; ++i){ 
        for(size_t j=0; j<detector_cell_count; ++j){
            image[i][j] += data[0].events[j];
        }
    }


    //180 degree backprojection.
    for(size_t i=0; i<detector_cell_count; ++i){
        for(size_t j=0; j<detector_cell_count; ++j){
            image[i][j] += data[0].events[detector_cell_count - (j+1)];
        }
    }
  
    //270 degree backprojection.
    for(size_t i=0; i<detector_cell_count; ++i){
        for(size_t j=0; j<detector_cell_count; ++j){
            image[i][j] += data[0].events[detector_cell_count - (i+1)];
        }
    }

    //90 degree backprojection.
    for(size_t i=0; i<detector_cell_count; ++i){
        for(size_t j=0; j<detector_cell_count; ++j){
            image[i][j] += data[0].events[i];
        }
    }
*/


    //---------------------------------------------------------------------------------------------------
    //---------------------------------------- Post-processing ------------------------------------------
    //---------------------------------------------------------------------------------------------------
 
    //Find the highest pixel value. Scale the others to this value (clamp the values.)
    double highest = 0.0;
    for(size_t i=0; i<detector_cell_count; ++i){
        for(size_t j=0; j<detector_cell_count; ++j){
            if(image[i][j] > highest) highest = image[i][j];
        }
    }

    if(highest > 0.0){
        for(size_t i=0; i<detector_cell_count; ++i){
            for(size_t j=0; j<detector_cell_count; ++j){
                image[i][j] /= highest;
            }
        }
    }

/*
    //Invert the intensity, as these things are normally done...
    if(highest > 0.0){
        for(size_t i=0; i<detector_cell_count; ++i){
            for(size_t j=0; j<detector_cell_count; ++j){
                image[i][j] = 1.0 - image[i][j];
            }
        }
    }
*/

/*
    //Print the data to screen.
    for(size_t i=0; i<detector_cell_count; ++i){    
        for(size_t j=0; j<detector_cell_count; ++j){
            printf(" %lf", image[i][j]);
        }
        std::cout << std::endl;
    }
   
*/


    //Eliminate negatives in the image data by setting them to zero. Better method?
    for(size_t i=0; i<detector_cell_count; ++i){
        for(size_t j=0; j<detector_cell_count; ++j){
            if(image[i][j] < 0.0) image[i][j] = 0.0;
        }
    }

    //---------------------------------------------------------------------------------------------------
    //-------------------------------- Output image / Image processing ----------------------------------
    //---------------------------------------------------------------------------------------------------
    //Write an image to file.
    Write_Image((int)(detector_cell_count), (int)(detector_cell_count), image, "notused right now");



    //---------------------------------------------------------------------------------------------------
    //--------------------------------------- Additional Metrics ----------------------------------------
    //---------------------------------------------------------------------------------------------------
    std::fstream Filep;

    //Produce an intensity profile along the central axes.
    Filep.open("/tmp/Transport_Central_Axis_Profiles.plot", std::ios::out );  //ios::out | ios::binary.

    if(!Filep.good()){
        std::cout << "Unable to open profile output file. Exiting." << std::endl;
        return -1;
    }

    Filep << "# Profiles along central axes. " << std::endl;
    Filep << "# pixel location   (vertical image axis)   (horizontal image axis) " << std::endl;
    for(size_t i=0; i<detector_cell_count; ++i){
        Filep << i << " " << image[i][detector_cell_count/2] << " " << image[detector_cell_count/2][i] << std::endl;
    }
    Filep.close();


    //Print out a detector array. This is to examine convolution, scaling, etc..
    Filep.open("/tmp/Transport_Detector_sample_convolved.plot", std::ios::out );  //ios::out | ios::binary.

    if(!Filep.good()){
        std::cout << "Unable to open profile output file. Exiting." << std::endl;
        return -1;
    }

    Filep << "# Pre-processed detector array. Possibly convoluted with a point-spread, blurred, normalized, or anything. Examine the source for details. " << std::endl;
    Filep << "# cell number     intensity " << std::endl;
    for(size_t i=0; i<detector_cell_count; ++i){
        Filep << i << " " << data[0].events[i] << std::endl;
    }
    Filep.close();

    //Print out a sample FFT of the original data.
    Filep.open("/tmp/Transport_Detector_sample_FFT.plot", std::ios::out );  //ios::out | ios::binary.

    if(!Filep.good()){
        std::cout << "Unable to open profile output file. Exiting." << std::endl;
        return -1;
    }

    Filep << "# Fourier transform of normalized detector array signal. Possibly blurred, normalized, or anything. Examine the source for details. " << std::endl;
    Filep << "# cell number     intensity(real)  intensity(imag) " << std::endl;
    for(size_t i=0; i<detector_cell_count; ++i){
        Filep << i << " " << fft_out_forward[i][0] << " " << fft_out_forward[i][1] << std::endl;
    }
    Filep.close();

    //Print out a sample of the (frequency-space) filtered FFT data.
    Filep.open("/tmp/Transport_Detector_sample_FFT_filtered.plot", std::ios::out );  //ios::out | ios::binary.

    if(!Filep.good()){
        std::cout << "Unable to open profile output file. Exiting." << std::endl;
        return -1;
    }

    Filep << "# Filtered Fourier transform of normalized detector array signal. Possibly blurred, normalized, or anything. Examine the source for details. " << std::endl;
    Filep << "# cell number     intensity(real)  intensity(imag) " << std::endl;
    for(size_t i=0; i<detector_cell_count; ++i){
        Filep << i << " " << fft_in_backward[i][0] << " " << fft_in_backward[i][1] << std::endl;
    }
    Filep.close();

   
    //Cleanup FFTW stuff.
    fftw_destroy_plan(fft_plan_forward);
    fftw_destroy_plan(fft_plan_backward);

    fftw_free(fft_in_forward);
    fftw_free(fft_out_forward);

    fftw_free(fft_in_backward); 
    fftw_free(fft_out_backward);

    return 0;
}



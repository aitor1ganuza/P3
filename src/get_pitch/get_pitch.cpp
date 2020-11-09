/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>
#include <algorithm>
#include "wavfile_mono.h"
#include "pitch_analyzer.h"
#include "math.h"
#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Detector 

Usage:
    get_pitch [options] <input-wav> <output-txt>
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -h, --help  Show this screen
    --version   Show the version of the project
    -1 FLOAT, --weight1=FLOAT  valor del peso weight1 [default: 0.92]
    -2 FLOAT, --weight2=FLOAT  valor del peso weight2 [default: 0.77]
    -3 FLOAT, --weight3=FLOAT  valor del peso weight3 [default: -60]
    

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the detection:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";

int main(int argc, const char *argv[]) {
	/// \TODO 
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

//debug, showing the elements of the map
#if 0
  std::cout << "DEBUG:" << "\n";
  for (auto elem: args) {
    std::cout << elem.first << " " << elem.second << "\n";
  }
  std::cout << "END DEBUG" << "\n";
  std::cout << std::stof(args["--weight2"].asString()) << endl;
  std::cout << "END DEBUG" << "\n";
#endif

	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();


  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer
  PitchAnalyzer analyzer(n_len, rate, std::stof(args["--weight1"].asString()), std::stof(args["--weight2"].asString()), std::stof(args["--weight3"].asString()), PitchAnalyzer::HAMMING, 50, 500);

  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  /// \DONE
  // Hacemos center-clipping
  vector<float>::iterator iX;
  #if 1
  float umbral = 0.005;
  for(iX = x.begin(); iX < x.end(); iX++) {
    if(*iX <= umbral && *iX >= -umbral) {
       *iX = 0;
    }/*
    else if(*iX > umbral) {
      *iX -= umbral;
    }
    else if(*iX < -umbral) {
      *iX += umbral;
    }*/
  }
  #endif
  // Iterate for each frame and save values in f0 vector
  vector<float> f0;
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    float f = analyzer(iX, iX + n_len);
    f0.push_back(f);
  }

  /// \TODO
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.
  //Utilizamos el median filter
  #if 0
  int longitud = static_cast<int>(f0.size());
  vector<float> ventana;
  cout << longitud << " " << '\n';
  for(int i = 0; i < longitud; i++) {
    //if(f0[i] != 0) {
      //Añadimos elementos a la ventana
      for(int j=0; j<5; j++) {
        ventana.insert(ventana.begin() + j, f0[i+j]);
        //if(ventana.)
      }
      //Poner aqui condición para si hay más de la mitad de 0
      //Ordenamos elementos
      sort(ventana.begin(),ventana.end());
      //Cogemos la mediana de la ventana
      
      f0[i] = ventana[2];
      ventana.clear();
    //}
  }
  #endif


  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}

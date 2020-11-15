/// @file

#ifndef PITCH_ANALYZER_H
#define PITCH_ANALYZER_H

#include <vector>
#include <algorithm>

namespace upc
{
  const float MIN_F0 = 20.0F;    ///< Minimum value of pitch in Hertzs
  const float MAX_F0 = 10000.0F; ///< Maximum value of pitch in Hertzs

  ///
  /// PitchAnalyzer: class that computes the pitch (in Hz) from a signal frame.
  /// No pre-processing or post-processing has been included
  ///
  class PitchAnalyzer
  {
  public:
    /// Window type
    enum Window
    {
      RECT,   ///< Rectangular window
      HAMMING ///< Hamming window
    };

    void set_window(Window type); ///< pre-compute window

  private:
    std::vector<float> window;       ///< precomputed window
    unsigned int frameLen,           ///< length of frame (in samples). Has to be set in the constructor call
        samplingFreq,                ///< sampling rate (in samples per second). Has to be set in the constructor call
        npitch_min,                  ///< minimum value of pitch period, in samples
        npitch_max;                  ///< maximum value of pitch period, in samples
    float weight1, weight2, weight3, weight4, weight5; ///< Los cinco parámetros para decidir si es voiced o unvoiced

    ///
    /// Computes correlation from lag=0 to r.size()
    ///
    void autocorrelation(const std::vector<float> &x, std::vector<float> &r) const;

    ///
    /// Returns the pitch (in Hz) of input frame x
    ///
    float compute_pitch(std::vector<float> &x) const;

    ///
    /// Returns true is the frame is unvoiced
    ///
    bool unvoiced(float pot, float r1norm, float rmaxnorm) const;

  public:
    PitchAnalyzer(unsigned int fLen,                 ///< Frame length in samples
                  unsigned int sFreq,                ///< Sampling rate in Hertzs
                  float w1,                          ///< parámetro 1 que se usa como umbral para voiced/unvoiced. También se podría usar como peso para ponderar r1norm, de ahí su nombre
                  float w2,                          ///< parámetro 2 que se usa como umbral para voiced/unvoiced.
                  float w3,                          ///< parámetro 3 que se usa como umbral para voiced/unvoiced.
                  float w4,                          ///< parámetro 4 que se usa como umbral para voiced/unvoiced.
                  float w5,                          ///< parámetro 5 que se usa como umbral para voiced/unvoiced.
                  Window w = PitchAnalyzer::HAMMING, ///< Window type
                  float min_F0 = MIN_F0,             ///< Pitch range should be restricted to be above this value
                  float max_F0 = MAX_F0              ///< Pitch range should be restricted to be below this value
    )
    {
      frameLen = fLen;
      samplingFreq = sFreq;
      set_f0_range(min_F0, max_F0);
      set_window(w);
      weight1 = w1;
      weight2 = w2;
      weight3 = w3;
      weight4 = w4;
      weight5 = w5;
    }

    ///
    /// Operator (): computes the pitch for the given vector x
    ///
    float operator()(const std::vector<float> &_x) const
    {
      if (_x.size() != frameLen)
        return -1.0F;

      std::vector<float> x(_x); //local copy of input frame
      return compute_pitch(x);
    }

    ///
    /// Operator (): computes the pitch for the given "C" vector (float *).
    /// N is the size of the vector pointer by pt.
    ///
    float operator()(const float *pt, unsigned int N) const
    {
      if (N != frameLen)
        return -1.0F;

      std::vector<float> x(N);          //local copy of input frame, size N
      std::copy(pt, pt + N, x.begin()); ///copy input values into local vector x
      return compute_pitch(x);
    }

    ///
    /// Operator (): computes the pitch for the given vector, expressed by the begin and end iterators
    ///
    float operator()(std::vector<float>::const_iterator begin, std::vector<float>::const_iterator end) const
    {

      if (end - begin != frameLen)
        return -1.0F;

      std::vector<float> x(end - begin); //local copy of input frame, size N
      std::copy(begin, end, x.begin());  //copy input values into local vector x
      return compute_pitch(x);
    }

    ///
    /// Sets pitch range: takes min_F0 and max_F0 in Hz, sets npitch_min and npitch_max in samples
    ///
    void set_f0_range(float min_F0, float max_F0);
  };
} // namespace upc
#endif

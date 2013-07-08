#include <systemc-ams.h>

SCA_TDF_MODULE(vco) {
  sca_tdf::sca_in<double> in;
  sca_tdf::sca_out<double> out;

  double gain;   // gain
  double kvco;   // sensitivity [Hz/V]
  double fc;     // central frequency [Hz]
  double vfc;    // control voltage to get FC

  double wc;     // central pulsation [rad/s]
  double kvcor;  // sensitivity [rad/(s*V)]
  bool debug_init;

  void init() {
    wc = 2.0 * M_PI * fc;
    kvcor = 2.0 * M_PI * kvco;
  }

  void processing() {
    if (debug_init) {
      std::cout << name() << ": "
        << " gain = " << gain
        << " kgco = " << kvco
        << " fc = " << fc
        << " vfc = " << vfc
        << " wc = " << wc
        << " kvcor = " << kvcor
        << std::endl;
      debug_init = 0;
    }

    double tn = sc_time_stamp().to_seconds();
    double wvco = (wc + kvcor * (in.read() - vfc));

    out.write(gain * sin(wvco * tn));
  }

  void set_attributes() {
    out.set_delay(1);  // feedback loop!
  }

  SCA_CTOR(vco) {
    debug_init = 1;
  }
};

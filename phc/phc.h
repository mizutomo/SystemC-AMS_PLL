#include <systemc-ams.h>

SCA_TDF_MODULE(phc) {
  sca_tdf::sca_in<double> in1;
  sca_tdf::sca_in<double> in2;

  sca_tdf::sca_out<double> out;

  double kpc;  // gain

  bool debug_init;

  void processing() {
    if (debug_init) {
      std::cout << name() << ": "
        << " kpc=" << kpc
        << std::endl;
      debug_init = 0;
    }

    out.write(kpc * in1.read() * in2.read());
  }

  SCA_CTOR(phc) {
    debug_init = 1;
  }
};

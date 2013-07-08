#include <systemc-ams.h>

SCA_TDF_MODULE(lp1) {
  sca_tdf::sca_in<double>  in;
  sca_tdf::sca_out<double> out;

  double fp;    // pole frequency
  double h0;    // DC gain

  double tau;   // time constant
  double outn1; // internal state
  double tn1;   // t(n-1)

  bool debug_init;

  void init() {
    tau = 1.0 / (2.0 * M_PI * fp);
  }

  void processing() {
    if (debug_init) {
      std::cout << name() << ": "
        << " FP=" << fp
        << " H0=" << h0
        << " TAU=" << tau
        << endl;
      debug_init = 0;
    }

    double tn = sc_time_stamp().to_seconds();
    double dt = tn - tn1;

    outn1 = (outn1 * tau + h0 * in.read() * dt) / (tau + dt);
    tn1 = tn;

    out.write(outn1);
  }

  SCA_CTOR(lp1) {
    outn1 = 0.0;
    tn1 = 0.0;
    debug_init = 1;
  }
};

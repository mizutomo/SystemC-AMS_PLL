#include <systemc-ams.h>
#include "phc.h"

SCA_TDF_MODULE(ref_src) {
  sca_tdf::sca_out<double> out;

  double ampl, freq;

  void processing() {
    double vout = ampl * sin(2 * M_PI * freq * sc_time_stamp().to_seconds());
    out.write(vout);
  }

  SCA_CTOR(ref_src) {}
};

SCA_TDF_MODULE(vco_src) {
  sca_tdf::sca_out<double> out;

  double ampl, freq;
  double dphi;   // Phase Shift

  void processing() {
    const double DPHI_STEP = 157.1e-3 * 0.05;   // per 1us*SDF step
    double vout = ampl * sin(2 * M_PI * freq * sc_time_stamp().to_seconds() + dphi);
    dphi += DPHI_STEP; dphi = (dphi > M_PI) ? M_PI : dphi;
    out.write(vout);
  }

  SCA_CTOR(vco_src) {
    dphi = -M_PI;
  }
};

int sc_main(int argc, char** argv) {
  sca_tdf::sca_signal<double> ref, vco, pco;

  sc_set_time_resolution(1.0, SC_NS);

  phc i_pc("pc");
  i_pc.in1(ref);
  i_pc.in2(vco);
  i_pc.out(pco);
  i_pc.kpc = 0.66;

  ref_src i_ref_src("ref_src");
  i_ref_src.out(ref);
  i_ref_src.set_timestep(0.05, SC_US);
  i_ref_src.ampl = 1.0;
  i_ref_src.freq = 1e6;

  vco_src i_vco_src("vco_src");
  i_vco_src.out(vco);
  i_vco_src.ampl = 1.0;
  i_vco_src.freq = 1e6;

  sca_trace_file* tr = sca_create_vcd_trace_file("phc");
  sca_trace(tr, ref, "ref");
  sca_trace(tr, vco, "vco");
  sca_trace(tr, pco, "pco");

  sc_start(41.0, SC_US);

  return 0;
}

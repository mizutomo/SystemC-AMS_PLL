#include <systemc-ams.h>
#include "lp1.h"

SCA_TDF_MODULE(src_sin) {
  sca_tdf::sca_out<double> out;

  double ampl, freq;

  void processing() {
    out.write(ampl * sin(2*M_PI*freq*sc_time_stamp().to_seconds()));
  }

  SCA_CTOR(src_sin) {}
};

int sc_main(int argc, char** argv) {
  sca_tdf::sca_signal<double> src, lpo;

  sc_set_time_resolution(1.0, SC_US);

  lp1 i_lp1("uut_lp1");
  i_lp1.in(src);
  i_lp1.out(lpo);
  i_lp1.fp = 1e3;
  i_lp1.h0 = 1.0;
  i_lp1.init();

  src_sin i_src("src");
  i_src.out(src);
  i_src.set_timestep(0.005, SC_MS);
  i_src.ampl = 1.0;
  i_src.freq = 1e4;

  sca_trace_file* tr = sca_create_vcd_trace_file("lp1");
  sca_trace(tr, src, "src");
  sca_trace(tr, lpo, "lpo");

  sc_start(2.0, SC_MS);

  return 0;
}

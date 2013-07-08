#include <systemc-ams.h>
#include "../phc/phc.h"
#include "../lp1/lp1.h"
#include "../vco/vco.h"

SCA_TDF_MODULE(src_sin) {
  sca_tdf::sca_out<double> out;

  double ampl, freq;

  void processing() {
    out.write(ampl * sin(2*M_PI*freq*sc_time_stamp().to_seconds()));
  }

  SCA_CTOR(src_sin) {}
};

int sc_main(int argc, char** argv) {
  sca_tdf::sca_signal<double> ref, pco, lpo, vcoo;
  sc_set_time_resolution(0.001, SC_US);

  phc i_phc("phc");
  i_phc.in1(ref);
  i_phc.in2(vcoo);
  i_phc.out(pco);
  i_phc.kpc = 3.72;

  lp1 i_lp1("lp1");
  i_lp1.in(pco);
  i_lp1.out(lpo);
  i_lp1.fp = 112e3;
  i_lp1.h0 = 1.0;
  i_lp1.init();

  vco i_vco("vco");
  i_vco.in(lpo);
  i_vco.out(vcoo);
  i_vco.gain = 1.0;
  i_vco.kvco = 3e4;
  i_vco.fc = 7e6;
  i_vco.vfc = 0.0;
  i_vco.init();

  src_sin src_ref("src_ref");
  src_ref.out(ref);
  src_ref.set_timestep(0.001, SC_US);
  src_ref.ampl = 1.0;
  src_ref.freq = 7e6;

  sca_trace_file* tr = sca_create_vcd_trace_file("pll");
  sca_trace(tr, ref, "ref");
  sca_trace(tr, pco, "pco");
  sca_trace(tr, lpo, "lpo");
  sca_trace(tr, vcoo, "vcoo");

  sc_start(120, SC_US);

  return 0;
}

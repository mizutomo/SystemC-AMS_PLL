#include <systemc-ams.h>
#include "vco.h"

SCA_TDF_MODULE(src_ctrl) {
  sca_tdf::sca_out<double> out;

  void init() {
    out.write(0.0);
  }

  void processing() {
    double now = sc_time_stamp().to_seconds();
    if (now < 5.0e-5) {
      out.write(-5.0);
    } else if (now < 1.0e-4) {
      out.write(+5.0);
    } else {
      out.write(0.0);
    }
  }

  void set_attributes() {
    out.set_delay(1);
  }

  SCA_CTOR(src_ctrl) {}
};

int sc_main(int argc, char** argv) {
  sca_tdf::sca_signal<double> ctrl, vcoo;

  sc_set_time_resolution(0.01, SC_US);

  vco i_vco("vco");
  i_vco.in(ctrl);
  i_vco.out(vcoo);
  i_vco.gain = 2.5;
  i_vco.kvco = 1e+5;
  i_vco.fc = 1e+6;
  i_vco.vfc = 0.0;
  i_vco.init();

  src_ctrl i_src("src_ctrl");
  i_src.out(ctrl);
  i_src.set_timestep(0.01, SC_US);

  sca_trace_file* tr = sca_create_vcd_trace_file("vco");
  sca_trace(tr, ctrl, "ctrl");
  sca_trace(tr, vcoo, "vcoo");

  sc_start(140, SC_US);

  return 0;
}

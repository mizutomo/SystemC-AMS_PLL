#include <systemc-ams.h>

#include "../phc/phc.h"
#include "../lp1/lp1.h"
#include "../vco/vco.h"

SCA_TDF_MODULE(pll) {
  sca_tdf::sca_in<double>  ref;
  sca_tdf::sca_out<double> lpo, vcoo;

  sca_tdf::sca_signal<double> pco;

  SC_CTOR(pll) {
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
    i_vco.out.set_delay(1);  // feedback loop!
    i_vco.gain = 1.0;
    i_vco.kvco = 3e4;
    i_vco.fc = 7e6;
    i_vco.vfc = 0.0;
    i_vco.init();
  }
};

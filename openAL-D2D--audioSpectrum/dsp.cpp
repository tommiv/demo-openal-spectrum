#include <windows.h>
#include "dsp.h"

pgetVersionDSP			getVersionDSP;

//**********************************************************************
pfftCreate				fftCreate;
pfftResize				fftResize;
pfftReal				fftReal;
pfftRealAbs				fftRealAbs;
pfftComplex				fftComplex;
pfftComplexAbs			fftComplexAbs;
pifftComplex			ifftComplex;
pifftReal				ifftReal;
pfftShift				fftShift;
pfftGetLength			fftGetLength;
pfftClear				fftClear;

//**********************************************************************
pconv					conv;
pcircularConv			circularConv;
pfilterGetOrder			filterGetOrder;
pfreqp					freqp;
pfreqz					freqz;
pfilterCreate			filterCreate;
pfilterClear			filterClear;
pfilterIIR				filterIIR;
pfilterFIR				filterFIR;
pfilterFIRCreate		filterFIRCreate;
pfilterGaussian			filterGaussian;

//**********************************************************************
pwindow					window;
pnormwin				normwin;

//**********************************************************************
philbert				hilbert;
poscillator				oscillator;
pam						am;
pdsb					dsb;
pssb					ssb;
ppm						pm;
pfm						fm;

prandBinData			randBinData;
pfsk					fsk;
pcpfsk					cpfsk;
pmsk					msk;
pgmsk					gmsk;

pfmDemodIQ				fmDemodIQ;
ppmDemodIQ				pmDemodIQ;
punwrap					unwrap;
pbpsk					bpsk;
pdata2stream			data2stream;

//**********************************************************************
psave2txt				save2txt;
pniReadIQ				niReadIQ;

//**********************************************************************
pmean					mean;
pstdev					stdev;
pmedian					median;

//**********************************************************************
pcicFilter				cicFilter;
pcicCorrectCreate		cicCorrectCreate;
pcicDecim				cicDecim;
pcicInterp				cicInterp;

//**********************************************************************
presampling				resampling;

//**********************************************************************
pasinh					asinh;
pacosh					acosh;
pellipk					ellipk;
pellipdeg				ellipdeg;
pcde					cde;
psne					sne;
plaguerre				laguerre;


HMODULE LoadDSP(){
	HMODULE hDLL=LoadLibrary(TEXT("DSP.DLL"));
	if(!hDLL) {
		return 0;
	}

	getVersionDSP = (pgetVersionDSP)GetProcAddress(hDLL, "getVersionDSP");
	if(!getVersionDSP) return 0;
		
	fftCreate = (pfftCreate)GetProcAddress(hDLL, "fftCreate");
	if(!fftCreate) return 0;
	
	fftResize = (pfftResize)GetProcAddress(hDLL, "fftResize");
	if(!fftResize) return 0;
	
	fftReal = (pfftReal)GetProcAddress(hDLL, "fftReal");
	if(!fftReal) return 0;

	fftRealAbs = (pfftRealAbs)GetProcAddress(hDLL, "fftRealAbs");
	if(!fftRealAbs) return 0;

	fftComplex = (pfftComplex)GetProcAddress(hDLL, "fftComplex");
	if(!fftComplex) return 0;

	fftComplexAbs = (pfftComplexAbs)GetProcAddress(hDLL, "fftComplexAbs");
	if(!fftComplexAbs) return 0;

	ifftComplex = (pifftComplex)GetProcAddress(hDLL, "ifftComplex");
	if(!ifftComplex) return 0;

	ifftReal = (pifftReal)GetProcAddress(hDLL, "ifftReal");
	if(!ifftReal) return 0;

	fftShift = (pfftShift)GetProcAddress(hDLL, "fftShift");
	if(!fftShift) return 0;

	fftGetLength = (pfftGetLength)GetProcAddress(hDLL, "fftGetLength");
	if(!fftGetLength) return 0;

	fftClear = (pfftClear)GetProcAddress(hDLL, "fftClear");
	if(!fftClear) return 0;


	// Filtering
	conv = (pconv)GetProcAddress(hDLL, "conv");
	if(!conv) return 0;

	circularConv = (pcircularConv)GetProcAddress(hDLL, "circularConv");
	if(!circularConv) return 0;

	filterGetOrder = (pfilterGetOrder)GetProcAddress(hDLL, "filterGetOrder");
	if(!filterGetOrder) return 0;

	freqp = (pfreqp)GetProcAddress(hDLL, "freqp");
	if(!freqp) return 0;

	freqz = (pfreqz)GetProcAddress(hDLL, "freqz");
	if(!freqz) return 0;

	filterCreate = (pfilterCreate)GetProcAddress(hDLL, "filterCreate");
	if(!filterCreate) return 0;

	filterClear = (pfilterClear)GetProcAddress(hDLL, "filterClear");
	if(!filterClear) return 0;

	filterIIR = (pfilterIIR)GetProcAddress(hDLL, "filterIIR");
	if(!filterIIR) return 0;

	filterFIR = (pfilterFIR)GetProcAddress(hDLL, "filterFIR");
	if(!filterFIR) return 0;

	filterFIRCreate = (pfilterFIRCreate)GetProcAddress(hDLL, "filterFIRCreate");
	if(!filterFIRCreate) return 0;
	
	filterGaussian = (pfilterGaussian)GetProcAddress(hDLL, "filterGaussian");
	if(!filterGaussian) return 0;

	// Window
	window = (pwindow)GetProcAddress(hDLL, "window");
	if(!window) return 0;

	normwin = (pnormwin)GetProcAddress(hDLL, "normwin");
	if(!normwin) return 0;


	// Modulation
	hilbert = (philbert)GetProcAddress(hDLL, "hilbert");
	if(!hilbert) return 0;

	oscillator = (poscillator)GetProcAddress(hDLL, "oscillator");
	if(!oscillator) return 0;

	am = (pam)GetProcAddress(hDLL, "am");
	if(!am) return 0;

	dsb = (pdsb)GetProcAddress(hDLL, "dsb");
	if(!dsb) return 0;

	ssb = (pssb)GetProcAddress(hDLL, "ssb");
	if(!ssb) return 0;

	pm = (ppm)GetProcAddress(hDLL, "pm");
	if(!pm) return 0;

	fm = (pfm)GetProcAddress(hDLL, "fm");
	if(!fm) return 0;

	randBinData = (prandBinData)GetProcAddress(hDLL, "randBinData");
	if(!randBinData) return 0;

	fsk = (pfsk)GetProcAddress(hDLL, "fsk");
	if(!fsk) return 0;

	cpfsk = (pcpfsk)GetProcAddress(hDLL, "cpfsk");
	if(!cpfsk) return 0;

	msk = (pmsk)GetProcAddress(hDLL, "msk");
	if(!msk) return 0;

	gmsk = (pgmsk)GetProcAddress(hDLL, "gmsk");
	if(!gmsk) return 0;

	fmDemodIQ = (pfmDemodIQ)GetProcAddress(hDLL, "fmDemodIQ");
	if(!fmDemodIQ) return 0;

	pmDemodIQ = (ppmDemodIQ)GetProcAddress(hDLL, "pmDemodIQ");
	if(!pmDemodIQ) return 0;

	unwrap = (punwrap)GetProcAddress(hDLL, "unwrap");
	if(!unwrap) return 0;

	bpsk = (pbpsk)GetProcAddress(hDLL, "bpsk");
	if(!bpsk) return 0;

	data2stream = (pdata2stream)GetProcAddress(hDLL, "data2stream");
	if(!data2stream) return 0;

	// IO
	save2txt = (psave2txt)GetProcAddress(hDLL, "save2txt");
	if(!save2txt) return 0;

	niReadIQ = (pniReadIQ)GetProcAddress(hDLL, "niReadIQ");
	if(!niReadIQ) return 0;


	// Stat
	mean = (pmean)GetProcAddress(hDLL, "mean");
	if(!mean) return 0;

	stdev = (pstdev)GetProcAddress(hDLL, "stdev");
	if(!stdev) return 0;

	median = (pmedian)GetProcAddress(hDLL, "median");
	if(!median) return 0;

	// CIC-filter
	cicFilter = (pcicFilter)GetProcAddress(hDLL, "cicFilter");
	if(!cicFilter) return 0;

	cicCorrectCreate = (pcicCorrectCreate)GetProcAddress(hDLL, "cicCorrectCreate");
	if(!cicCorrectCreate) return 0;

	cicDecim = (pcicDecim)GetProcAddress(hDLL, "cicDecim");
	if(!cicDecim) return 0;

	cicInterp = (pcicInterp)GetProcAddress(hDLL, "cicInterp");
	if(!cicInterp) return 0;


	// Resampling
	resampling = (presampling)GetProcAddress(hDLL, "resampling");
	if(!resampling) return 0;


	// Helpers
	asinh = (pasinh)GetProcAddress(hDLL, "asinh");
	if(!asinh) return 0;

	acosh = (pacosh)GetProcAddress(hDLL, "acosh");
	if(!asinh) return 0;

	ellipk = (pellipk)GetProcAddress(hDLL, "ellipk");
	if(!ellipk) return 0;

	ellipdeg = (pellipdeg)GetProcAddress(hDLL, "ellipdeg");
	if(!ellipdeg) return 0;

	cde = (pcde)GetProcAddress(hDLL, "cde");
	if(!cde) return 0;

	sne = (psne)GetProcAddress(hDLL, "sne");
	if(!sne) return 0;

	laguerre = (plaguerre)GetProcAddress(hDLL, "laguerre");
	if(!laguerre) return 0;

	return hDLL;	
}



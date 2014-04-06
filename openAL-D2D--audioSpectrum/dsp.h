#ifndef DSP_H
#define DSP_H

#include <windows.h>

#define DSP_OK					 0
#define DSP_ERROR				-1


typedef DWORD (*pgetVersionDSP)(void);

extern pgetVersionDSP			getVersionDSP;

//**********************************************************************************************************************
typedef int	(*pfftCreate)(int n,     void** vfft);
typedef int	(*pfftResize)(int n,     void** vfft);
typedef int	(*pfftReal)(double *s, double *SR, double *SI, int n, void* vfft);
typedef int	(*pfftRealAbs)(double*s, double* S, int n, void * vfft);
typedef int	(*pfftComplex)(double *sr, double *si, double *SR, double *SI, int n, void * vfft);
typedef int	(*pfftComplexAbs)(double *sr, double *si, double *S, int n, void * vfft);
typedef int	(*pifftComplex)(double *SR, double *SI, double *sr, double *si, int n, void * vfft);
typedef int	(*pifftReal)(double *SR, double *SI, double *sr, int n, void * vfft);
typedef int	(*pfftShift)(double *SR, double *SI, int n);
typedef int	(*pfftGetLength)(void * vfft);
typedef void (*pfftClear)(void** vfft);

extern pfftCreate				fftCreate;
extern pfftResize				fftResize;
extern pfftReal					fftReal;
extern pfftRealAbs				fftRealAbs;
extern pfftComplex				fftComplex;
extern pfftComplexAbs			fftComplexAbs;
extern pifftComplex				ifftComplex;
extern pifftReal				ifftReal;
extern pfftShift				fftShift;
extern pfftGetLength			fftGetLength;
extern pfftClear				fftClear;


//**********************************************************************************************************************
#define FA_TYPE 				0x0F
#define FA_BUTTER 				0x01
#define FA_CHEBY1 				0x02
#define FA_CHEBY2  				0x04
#define FA_ELLIP 				0x08

#define FB_TYPE 				0xF0
#define FB_LOW 					0x10
#define FB_HIGH 				0x20
#define FB_BPASS  				0x40
#define FB_BSTOP 				0x80

//****************************************************************************************************************************
// структура задает коридор АЧХ для аналогового и цифрового фильтров
typedef struct{
		bool digital;	//флаг если true то фильтр цифрового если false, то аналоговый
		char type;		//тип фильтра объединяет флаги FA_***, 
						// которые указывают на тип аппроксимации аналогового фильтра и флагов FB_***,
						// которые задают тип фильтра: ФНЧ, ФВЧ, ПФ или РФ
		double wpl;		// нижняя частота пропускания
		double wsl;		// нижняя частота загражедния
		double wph;		// верхняя частота пропускания (используется в случае ПФ и РФ)
		double wsh;		// верхняя частота заграждения (используется в случае ПФ и РФ)
		double Rp;		// неравномерность АЧХ в полосе пропускания дБ
		double Rs;		// уровень подавления в полосе заграждения
}FP, *PFP;
 
typedef struct{
		double* b;
		double* a;
		int Nb;
		int Na;
}FC, *PFC;

typedef int	 (*pconv)(double *a, int Na, double *b, int Nb, double *c);
typedef int  (*pcircularConv)(double *a, double *b, double *c, int N, void* pfft);
typedef int  (*pfilterGetOrder)(PFP pfp);
typedef int  (*pfreqp)(PFC pfc, double* w, double* Hr, double* Hi, int N);	//+++ расчет комплексного к-та передачи аналогового фильтра H(jw)
typedef int  (*pfreqz)(PFC pfc, double* w, double* Hr, double* Hi, int N);	// расчет комплексного к-та передачи цифрового фильтра H(exp(jw))		
typedef int  (*pfilterCreate)(PFP pfp, PFC pfc, int fixedOrder);			//функция расчета фильтра по коридору АЧХ	
typedef void (*pfilterClear)(PFC pfc);
typedef int  (*pfilterIIR)(PFC pfc, double *s, int n);
typedef int  (*pfilterFIR)(double *s, int Ns, double* h, int Nh, int shift);
typedef int  (*pfilterFIRCreate)(char fType, double wpl, double wph, double* h, int Nh, int wType);
typedef int  (*pfilterGaussian)(double* g, int Lg, double BT, int bc);

extern pconv					conv;
extern pcircularConv			circularConv;
extern pfilterGetOrder			filterGetOrder;
extern pfreqp					freqp;
extern pfreqz					freqz;
extern pfilterCreate			filterCreate;
extern pfilterClear				filterClear;
extern pfilterIIR				filterIIR;
extern pfilterFIR				filterFIR;
extern pfilterFIRCreate			filterFIRCreate;
extern pfilterGaussian			filterGaussian;

//**********************************************************************************************************************
// оконные функции
#define WIN_RECT				0				
#define WIN_HAMMING				1		
#define WIN_HANN				2				
#define WIN_SIN					3			
#define WIN_LANCZOS				4		
#define WIN_BARLETT				5			
#define WIN_GAUSS				6			
#define WIN_BARLETT_HANN		7		
#define WIN_BLACKMAN			8		
#define WIN_NUTTAL				9			
#define WIN_BLACKMAN_NUTTAL		10	
#define WIN_BLACKMAN_HARRIS		11
#define WIN_FLAT_TOP			12		

typedef int (*pwindow)(double* w, int n, int winType, double param);
typedef void (*pnormwin)(double *w, int N);

extern pwindow					window;
extern pnormwin					normwin;

//**********************************************************************************************************************

typedef int (*philbert)(double* s, double* sr, double* si, int n, void* pfft);
typedef int (*poscillator)(double* s, double* I, double* Q, int N, double Fs, double f0, double band, double phi0);
typedef int (*pam) (double* t, double* sm, double* s, int N, double f0, double m);
typedef int (*pdsb)(double* t, double* sm, double* s, int N, double f0);	
typedef int (*pssb)(double* t, double* sm, double* s, int N, double f0, bool side);
typedef int (*ppm) (double* t, double* sm, double* s, int N, double f0, double m);
typedef int (*pfm) (double* t, double* sm, double* s, int N, double f0, double fd);


typedef int (*pfsk)(double* t, double* b, double* s, int N, double f0, double baudRate, double m);
typedef int (*pcpfsk)(double* t, double* b, double* s, int N, double f0, double baudRate, double m);
typedef int (*pmsk)(double* t, double* b, double* s, int N, double f0, double baudRate);
typedef int (*pgmsk)(double* t, double* b, double* s, int N, double f0, double Br, double BT);

typedef int (*pbpsk)(double* t, double* b, double* s, int N, double f0, double phi);

typedef int (*pfmDemodIQ)(double* I, double* Q, double* sd, int N, double Fs);
typedef int (*ppmDemodIQ)(double* I, double* Q, double* sd, int N, bool unwrapFlag);
typedef int (*punwrap)(double* s, double* su, int N, double d, double k);

typedef int (*prandBinData)(double* b, int N, double Fs, double Fd, bool randStart);
typedef int (*pdata2stream)(double* b, int Nb, char* data, int Nd, double br, double Fs);

extern philbert					hilbert;
extern poscillator				oscillator	;
extern pam						am	;
extern pdsb						dsb	;
extern pssb						ssb	;
extern ppm						pm	;
extern pfm						fm	;

extern pfsk						fsk;		
extern pcpfsk					cpfsk;
extern pmsk						msk;
extern pgmsk					gmsk;
extern pbpsk					bpsk;

extern pfmDemodIQ				fmDemodIQ;
extern ppmDemodIQ				pmDemodIQ;
extern punwrap					unwrap;

extern prandBinData				randBinData	;
extern pdata2stream				data2stream;

//**********************************************************************************************************************

typedef int (*psave2txt)(double* t, double* s, int n, char* fileName);
typedef int (*pniReadIQ)(HANDLE hFile, double* I, double* Q, int n, int pos);

extern psave2txt				save2txt;
extern pniReadIQ				niReadIQ;


//**********************************************************************************************************************
// функции статистики
typedef  double (*pmean)(double* s, int n);
typedef  double (*pstdev)(double* s, int n, int flag);
typedef  double (*pmedian)(double* s, int n);

extern pmean					mean;
extern pstdev					stdev;
extern pmedian					median;


//**********************************************************************************************************************
// функции моделирования CIC-фильтров
typedef  int (*pcicFilter)(double* s, int N, int K, int D);
typedef  int (*pcicCorrectCreate)(double *h, int Nh, int K, int R, void* pfft);
typedef  int (*pcicDecim)(double* s, int N, int K, int D, double *sd, bool flagCorrect, int P);
typedef  int (*pcicInterp)(double* s, int N, int K, int D, double *si, bool flagCorrect, int P);

extern pcicFilter				cicFilter		;
extern pcicCorrectCreate		cicCorrectCreate;
extern pcicDecim				cicDecim		;
extern pcicInterp				cicInterp		; 


//**********************************************************************************************************************
typedef  int (*presampling)(double* s, int N, double fs, double* sr, int Nr, double fsr);

extern presampling				resampling;


//**********************************************************************************************************************
typedef double (*pasinh)(double x);						
typedef double (*pacosh)(double x);						
typedef double (*pellipk)(double x);					
typedef double (*pellipdeg)(int N, double k1);
typedef double (*pcde)(double  u, double k, int iter);
typedef double (*psne)(double  u, double k, int iter);
typedef double (*plaguerre)(int N, int alpha, double x);

extern pasinh					asinh	;	
extern pacosh					acosh	;
extern pellipk					ellipk	;
extern pellipdeg				ellipdeg;
extern pcde						cde		;
extern psne						sne		;
extern plaguerre				laguerre;
//**********************************************************************************************************************
HMODULE LoadDSP();


#endif



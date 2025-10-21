#ifndef STDMACROS_H_
#define STDMACROS_H_
#define setbit(reg,bit) reg|=(1<<bit)
#define clearbit(reg,bit) reg&=~(1<<bit)
#define toggelbit(reg,bit) reg^=(1<<bit)
#define readbit(reg,bit) ((reg>>bit)&1)




#endif /* STDMACROS_H_ */

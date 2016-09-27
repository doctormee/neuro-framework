#include "neuro.h"
#include <math.h>
double sygmoid( double n ){
    return 1 / (1 + exp( -n ) );
}
double dsyg( double n ){
    return sygmoid( n ) * (1 - sygmoid( n ) );
}

int main(){
    Net* net1 = new Net(2, 2, 3, 1, sygmoid, dsyg, sygmoid, dsyg );
   // Dlist tst1, ans1, tst2, ans2, tst3, tst4;
    /*tst1.append(1);
    tst1.append(0);
    tst2.append(0);
    tst2.append(1);
    tst3.append(1);
    tst3.append(1);
    tst4.append(0);
    tst4.append(0);
    ans1.append(1);
    ans2.append(0);*/
    for (int i = 1; i <= 1000; ++i){
        /*net1.back_propagate(  tst1,ans1,1, 1);
        net1.back_propagate(  tst2,ans1,1, 1);
        net1.back_propagate(  tst3,ans2,1, 1);
        net1.back_propagate(  tst4,ans2,1, 1);*/
    }
    delete net1;
    return 0;
}
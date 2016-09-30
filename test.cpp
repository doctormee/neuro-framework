#include "neuro.h"
#include <math.h>
double sygmoid( double n ){
    return 1 / (1 + exp( -1 * n ) );
}
double dsyg( double n ){
    return sygmoid( n ) * (1 - sygmoid( n ) );
}

int main(){
    Net* net1 = new Net(2, 2, 2, 1, sygmoid, dsyg, sygmoid, dsyg );
     Dlist tst1, ans1, tst2, ans2, tst3, tst4, ans;
    tst1.append(1);
    tst1.append(0);
    tst2.append(1);
    tst2.append(1);
    tst3.append(0);
    tst3.append(1);
    tst4.append(0);
    tst4.append(0);
    ans1.append(1);
    ans2.append(0);
    for (int i = 1; i <= 100; ++i){
        net1->back_propagate(  tst1,ans1,10, 0.1);
        net1->back_propagate(  tst2,ans2,10, 0.1);
        net1->back_propagate(  tst3,ans1,10, 0.1);
        net1->back_propagate(  tst4,ans2,10, 0.1);
    }
    ans = net1->activate( tst2 );
    std::cout << ( ans.elem )<< '\n'; 
    delete net1;
    return 0;
}
#include "neuro.h"
#include <math.h>
double sygmoid( double n ){
    return 1 / (1 + exp( -1 * n ) );
}
double dsyg( double n ){
    return sygmoid( n ) * (1 - sygmoid( n ) );
}

int main(){
    Net* net1 = new Net(3, 2, 2, 1, sygmoid, dsyg, sygmoid, dsyg );
     Dlist tst1, ans1, tst2, ans2, tst3, tst4, ans;
    tst1.append(1.7);
    tst1.append(-1.7);
    tst2.append(-23);
    tst2.append(-37.544);
    tst3.append(1.000922);
    tst3.append(2.31231);
    tst4.append(-32);
    tst4.append(-32.00002);
    ans1.append(1);
    ans2.append(0);
    for (int i = 1; i <= 1; ++i){
        net1->back_propagate(  tst1,ans1,2, 0.01);
        net1->back_propagate(  tst2,ans1,2, 0.01);
        net1->back_propagate(  tst3,ans2,2, 0.01);
        net1->back_propagate(  tst4,ans2,2, 0.01);
    }
    ans = net1->activate( tst3 );
    std::cout << ( ans.elem )<< '\n';
    delete net1;
    return 0;
}
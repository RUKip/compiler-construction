int b;
int a;

int gcd_recursive (int v, int u){
int t0 = u ;
int t1 = v ;
int t2 = 0 ;
int t3 = v ;
int t4 = u ;
int t5 = v ;
int t6 = gcd_recursive (t3, t5);
int t7 = u ;
int t8 = v ;
int t9 = t8 ;
int t10 = v ;
int t11 = gcd_recursive t10 }
 
int gcd_iterative (int v, int u){
int t;
int t12 = v ;
int t13 = 0 ;
int t14 = u ;
int t15 = v ;
int t16 = t ;
int t17 = v ;
int t18 = u ;
}
 
int main(void){
int t19 = a ;
int t20 = b ;
int t21 = a ;
int t22 = b ;
int t23 = gcd_recursive (t21, t22);
int t24 = a ;
int t25 = b ;
int t26 = gcd_iterative (t24, t25);
return 0; 
 }
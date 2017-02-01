int b;
int a;

int gcd_recursive (int v, int u){
int t0 = u ;
int t1 = v ;
int t2 = t0 % t1;
int t3 = 0 ;
int t4 = t2<> 0 THEN t3;
int t5 = v ;
int t6 = u ;
int t7 = v ;
int t8 = t6 % t7;
int t9 = gcd_recursive (t5, t8);
int t10 = u ;
int t11 = v ;
int t12 = t10 +  t11;
int t13 = t12 ;
int t14 = v ;
int t15 = + t14 ;
}
 
int gcd_iterative (int v, int u){
int t;
int t16 = v ;
int t17 = 0 ;
int t18 = t16<> 0 do t17;
int t19 = u ;
int t20 = v ;
int t21 = t ;
int t22 = v ;
int t23 = t21 % t22;
int t24 = u ;
}
 
int main(void){
int t25 = a ;
int t26 = b ;
int t27 = a ;
int t28 = b ;
int t29 = gcd_recursive (t27, t28);
int t30 = a ;
int t31 = b ;
int t32 = gcd_iterative (t30, t31);
return 0; 
 }
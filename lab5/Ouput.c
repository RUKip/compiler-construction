int b;
int a;

int gcd_recursive (int v, int u){
int t0 = u ;
int t1 = v ;
int t2 = t0  % t1 ;
int t3 = 0 ;
int t4 = t2 <> t3 ;
if(! t4 ) goto  lab1;
int t5 = v ;
int t6 = u ;
int t7 = v ;
int t8 = t6  % t7 ;
int t9 = gcd_recursive (t5, t8);
gcd_recursive  =  t9 ;
goto  lab2;
lab1:  ;
int t10 = u ;
int t11 = v ;
int t12 = t10  +  t11 ;
int t13 = t12 ;
a  =  t13 ;
lab2:  ;
int t14 = v ;
int t15 = + t14 ;
gcd_recursive  =  t15 ;
return  gcd_recursive ;
}
 
int gcd_iterative (int v, int u){
int t;
int t16 = v ;
int t17 = 0 ;
int t18 = t16 <> t17 ;
lab3:  ;
if(! t18 ) goto  lab4;
int t19 = u ;
t  =  t19 ;
int t20 = v ;
u  =  t20 ;
int t21 = t ;
int t22 = v ;
int t23 = t21  % t22 ;
v  =  t23 ;
goto  lab3;
lab4:  ;
int t24 = u ;
gcd_iterative  =  t24 ;
return  gcd_iterative ;
}
 
int main(void){
int t25 = a ;
int t26 = b ;
scanf( " %d %d \n" , a, b) ;
int t27 = a ;
int t28 = b ;
int t29 = gcd_recursive (t27, t28);
int t30 = a ;
printf( " %d %d \n" , t29, t30) ;
int t31 = a ;
int t32 = b ;
int t33 = gcd_iterative (t31, t32);
printf( " %d \n" , t33) ;
return 0; 
 }
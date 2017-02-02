{ This is the Pasal implementation 
  of Euclid's algorithm for computing 
  the greatest common divisor of two integers.
}

{ a recursive version }
PROGRAM euclid(input, output);
  var a,b : integer;

FUNCTION gcd_recursive(u, v : integer) : integer;
BEGIN
  IF u mod v <> 0 THEN
    gcd_recursive := gcd_recursive(v, u mod v)
  ELSE
    a := (u + v);
    gcd_recursive := +v
END;

{ an iterative version }
procedure gcd_iterative(x, y, m, z : integer);
begin
   if x < y then
      m := x
   else
      m := y;
   
   if z <m then
      m := z;
end;

{ main program starts here }
BEGIN
  readln(a, b);
  writeln(gcd_recursive(a,b))
END.

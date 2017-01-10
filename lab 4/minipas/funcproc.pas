PROGRAM fp (input, output);

procedure procwithoutpars;
var x : integer;

BEGIN
   x := 42
END;

procedure procwithpars(x : integer; y  : real);
var p : integer;
var q : real;
BEGIN
   p := x;
   q := y
END;


function identity(x : real) : integer;
BEGIN
   identity := x
END;

BEGIN
   procwithoutpars;
   procwithpars (4,3);
   writeln(identity(42))
END.

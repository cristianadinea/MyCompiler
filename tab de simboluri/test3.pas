program tt3;
var
 a,b,c:real;
 i:char;
 function f(var a,b,c:real):integer;
  function f1(i:integer):real;
  var
   i1:integer;
  begin
   a:=a+i;
  end;
 begin
  a(a,b,c+i);
 end;
 procedure p;
  var x,y,z:integer;
  procedure p1;
   var x,y,z:integer;
  begin
   p;
   p1;
   x:=y+z+f1;
  end;
  procedure p2;
   var x,y,z:integer;
  begin
   p;p1;p2;
  end;
 begin
  p1;
  y:=x+z+f;
 end;
begin
 p;p1;f;f1;
end.
